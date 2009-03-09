
#include <entity.hpp>

namespace iq
{
	entity::entity(const std::string &path):
		animations(new entity::animation_map()),
		h(0),
		w(0),
		x(0),
		y(0)
	{
		this->load(path);
	}

	entity::entity(const std::string &path, const unsigned int w, const unsigned int h):
		animations(new entity::animation_map()),
		h(h),
		w(w),
		x(0),
		y(0)
	{
		this->load(path);
	}

	const boost::shared_ptr< std::vector<std::string> > entity::animation_keys(void) const
	{
		int j=0;
		boost::shared_ptr< std::vector<std::string> > keys(new std::vector<std::string>(this->animations->size()));

		for(entity::animation_map::iterator i=this->animations->begin(); i != this->animations->end(); i++)
			(*keys)[j] = i->first;

		return(keys);
	}

	const entity::animation_map::const_iterator entity::begin_animation(const std::string &key, const unsigned int ms)
	{
		animation_map::const_iterator animation_iterator = this->animations->find(key);

		if(animation_iterator == this->animations->end())
			throw std::range_error("Invalid animation key '" + key + "'.");

		this->current_animation_iterator = animation_iterator;

		return(this->current_animation_iterator);
	}

	const entity::animation_map::const_iterator entity::current_animation(void) const
	{
		return(this->current_animation_iterator);
	}

	const boost::shared_ptr<BITMAP> entity::current_frame(const unsigned int ms) const
	{
		return(this->current_animation()->second->next(ms));
	}

	void entity::load(const std::string &path)
	{
		boost::shared_ptr<TiXmlDocument> doc(new TiXmlDocument(path));
		TiXmlElement *root = NULL;
		TiXmlElement *element = NULL;

		if(!doc->LoadFile())
			throw std::runtime_error("Failed to load entity XML document '" + path + "'.");

		if((root = doc->RootElement()) == NULL)
			throw std::runtime_error("Entity XML document '" + path + "' missing root element.");

		if((element = root->FirstChildElement("spritesheet")) == NULL)
			throw std::runtime_error("Entity XML root element missing spritesheet element.");

		this->load_spritesheet(element);
	}

	void entity::load_spritesheet(const TiXmlElement *spritesheet)
	{
		boost::shared_ptr<unsigned int> sheet_ms_per_frame(new unsigned int());
		boost::shared_ptr<iq::animation> animation;
		boost::shared_ptr<iq::spritesheet> sheet;
		const char *file = NULL;
		unsigned int h, w;
		unsigned int ms_per_frame;
		std::string value;
		const TiXmlElement *element = NULL;
		const TiXmlNode *node = NULL;
		//const TiXmlText *text = NULL;

		if((file = spritesheet->Attribute("file")) == NULL)
			throw std::runtime_error("Entity XML spritesheet element missing file attribute.");

		if(spritesheet->Attribute("height", (int *)&h) == NULL)
			throw std::runtime_error("Entity XML spritesheet element missing height attribute.");

		if(spritesheet->Attribute("width", (int *)&w) == NULL)
			throw std::runtime_error("Entity XML spritesheet element missing width attribute.");

		if(spritesheet->Attribute("ms_per_frame", (int *)sheet_ms_per_frame.get()) == NULL)
			sheet_ms_per_frame.reset((unsigned int *)NULL);

		if((element = spritesheet->FirstChildElement("animation")) == NULL)
			throw std::runtime_error("Entity XML spritesheet element missing animation element(s).");

		sheet.reset(new iq::spritesheet(file, w, h));

		for(unsigned int i=0; i<sheet->h; i++)
		{
			if(element == NULL)
				throw std::runtime_error("Entity XML spritesheet element missing animation element(s).");

			if(element->Attribute("ms_per_frame", (int *)&ms_per_frame) == NULL && sheet_ms_per_frame.get() == NULL)
				throw std::runtime_error("Entity XML spritesheet element and animation element missing ms_per_frame attribute.");
			else
				ms_per_frame = *sheet_ms_per_frame;

			if((node = element->FirstChild()) == NULL || node->Type() != TiXmlNode::TEXT)
				throw std::runtime_error("Entity XML animation node missing key (name) as text node.");

			animation.reset(new iq::animation(ms_per_frame, sheet, i));

			value = node->ToText()->Value();
			boost::algorithm::trim(value);

			if(value.length() == 0)
				throw std::runtime_error("Entity XML animation node text node (key/name) is empty.");

			(*this->animations)[value] = animation;

			element = element->NextSiblingElement("animation");
		}
	}
}

