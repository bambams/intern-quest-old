
#include <entity.hpp>

namespace iq
{
	entity::entity(const std::string &path):
		m_animations(new entity::animation_map()),
		h(0),
		w(0),
		x(0),
		y(0)
	{
		this->load(path);
	}

	entity::entity(const std::string &path, const unsigned int w, const unsigned int h):
		m_animations(new entity::animation_map()),
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
		boost::shared_ptr< std::vector<std::string> > keys(new std::vector<std::string>(this->m_animations->size()));

		for(entity::animation_map::iterator i=this->m_animations->begin(); i != this->m_animations->end(); i++)
			(*keys)[j++] = i->first;

		return(keys);
	}

	const entity::animation_map::const_iterator entity::begin_animation(const std::string &key, const unsigned int ms)
	{
		animation_map::const_iterator animation_iterator = this->m_animations->find(key);

		if(animation_iterator == this->m_animations->end())
			throw std::range_error("Invalid animation key '" + key + "'.");

		this->m_current_animation_iterator = animation_iterator;

		return(this->m_current_animation_iterator);
	}

	const entity::animation_map::const_iterator entity::current_animation(void) const
	{
		return(this->m_current_animation_iterator);
	}

	const boost::shared_ptr<BITMAP> entity::current_frame(const unsigned int ms) const
	{
		return(this->current_animation()->second->next(ms));
	}

	void entity::load(const std::string &path)
	{
		boost::shared_ptr<BITMAP> bitmap;
		boost::shared_ptr<TiXmlDocument> doc(new TiXmlDocument(path));
		unsigned int w, h;
		TiXmlElement *root = NULL;
		TiXmlElement *element = NULL;

		if(!doc->LoadFile())
			throw std::runtime_error("Failed to load entity XML document '" + path + "'.");

		if((root = doc->RootElement()) == NULL)
			throw std::runtime_error("Entity XML document '" + path + "' missing root element.");

		if(root->Attribute("height", (int *)&h) != NULL)
			this->h = h;

		if(root->Attribute("width", (int *)&w) != NULL)
			this->w = w;

		if((element = root->FirstChildElement("spritesheet")) == NULL)
			throw std::runtime_error("Entity XML root element missing spritesheet element.");

		this->load_spritesheet(element);

		if(this->h == 0 && this->w == 0)
		{
			bitmap = this->m_animations->begin()->second->frame(0);

			this->h = bitmap->h;
			this->w = bitmap->w;
		}
	}

	void entity::load_animation(const unsigned int &i, const TiXmlElement * const animation_element, const boost::shared_ptr<iq::spritesheet> sheet, const boost::shared_ptr<unsigned int> anim_w, const boost::shared_ptr<unsigned int> anim_h, const boost::shared_ptr<unsigned int> sheet_ms_per_frame)
	{
		boost::shared_ptr<iq::animation> animation;
		std::string value;
		const TiXmlNode *node = NULL;
		unsigned int ms_per_frame;

		if(animation_element->Attribute("ms_per_frame", (int *)&ms_per_frame) == NULL && sheet_ms_per_frame.get() == NULL)
			throw std::runtime_error("Entity XML spritesheet element and animation element missing ms_per_frame attribute.");
		else
			ms_per_frame = *sheet_ms_per_frame;

		if((node = animation_element->FirstChild()) == NULL || node->Type() != TiXmlNode::TEXT)
			throw std::runtime_error("Entity XML animation node missing key (name) as text node.");

		if((anim_h.get() == NULL) != (anim_w.get() == NULL))
			throw std::runtime_error("Entity XML spritesheet attributes anim_width and anim_height must either both exist or neither can exist.");
		else if(anim_h.get() == NULL)
			animation.reset(new iq::animation(ms_per_frame, sheet, i));
		else
			animation.reset(new iq::animation(ms_per_frame, sheet, i, *anim_w, *anim_h));

		value = node->ToText()->Value();
		boost::algorithm::trim(value);

		if(value.length() == 0)
			throw std::runtime_error("Entity XML animation node text node (key/name) is empty.");

		(*this->m_animations)[value] = animation;
	}

	void entity::load_spritesheet(const TiXmlElement * const spritesheet_element)
	{
		boost::shared_ptr<unsigned int> sheet_ms_per_frame(new unsigned int()),
		                                            anim_w(new unsigned int()),
		                                            anim_h(new unsigned int());
		boost::shared_ptr<iq::spritesheet> sheet;
		const char *file = NULL;
		unsigned int h, w;
		const TiXmlElement *element = NULL;

		if((file = spritesheet_element->Attribute("file")) == NULL)
			throw std::runtime_error("Entity XML spritesheet element missing file attribute.");

		if(spritesheet_element->Attribute("height", (int *)&h) == NULL)
			throw std::runtime_error("Entity XML spritesheet element missing height attribute.");

		if(spritesheet_element->Attribute("width", (int *)&w) == NULL)
			throw std::runtime_error("Entity XML spritesheet element missing width attribute.");

		if(spritesheet_element->Attribute("anim_height", (int *)anim_h.get()) == NULL)
			anim_h.reset((unsigned int *)NULL);

		if(spritesheet_element->Attribute("anim_width", (int *)anim_w.get()) == NULL)
			anim_w.reset((unsigned int *)NULL);

		if((anim_h.get() == NULL) != (anim_w.get() == NULL))
			throw std::runtime_error("Entity XML spritesheet attributes anim_width and anim_height must either both exist or neither can exist.");

		if(spritesheet_element->Attribute("ms_per_frame", (int *)sheet_ms_per_frame.get()) == NULL)
			sheet_ms_per_frame.reset((unsigned int *)NULL);

		if((element = spritesheet_element->FirstChildElement("animation")) == NULL)
			throw std::runtime_error("Entity XML spritesheet element missing animation element(s).");

		sheet.reset(new iq::spritesheet(file, w, h));

		for(unsigned int i=0; i<sheet->h; i++)
		{
			if(element == NULL)
				throw std::runtime_error("Entity XML spritesheet element missing animation element(s).");

			this->load_animation(i, element, sheet, anim_w, anim_h, sheet_ms_per_frame);

			element = element->NextSiblingElement("animation");
		}
	}
}

