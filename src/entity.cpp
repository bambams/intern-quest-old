
#include <entity.hpp>

namespace iq
{
	const entity::facing_direction entity::DEFAULT_FACING = entity::FACING_DOWN;
	const unsigned int entity::DEFAULT_SPEED = 18;

	entity::entity(const std::string &path):
		m_animations(new entity::animation_map()),
		m_facing(entity::DEFAULT_FACING),
		m_h(0),
		m_w(0),
		m_x(0),
		m_y(0)
	{
		this->m_current_animation_iterator = this->m_animations->end();
		this->load(path);
	}

	entity::entity(const TiXmlElement * const entity):
		m_animations(new entity::animation_map()),
		m_facing(entity::DEFAULT_FACING),
		m_h(0),
		m_w(0),
		m_x(0),
		m_y(0)
	{
		this->m_current_animation_iterator = this->m_animations->end();
		this->load(entity);
	}

	entity::entity(const std::string &path, const unsigned int w, const unsigned int h):
		m_animations(new entity::animation_map()),
		m_facing(entity::DEFAULT_FACING),
		m_h(h),
		m_w(w),
		m_x(0),
		m_y(0)
	{
		this->m_current_animation_iterator = this->m_animations->end();
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

if(key == "walk_up")
	this->m_facing = entity::FACING_UP;
else if(key == "walk_right")
	this->m_facing = entity::FACING_RIGHT;
else if(key == "walk_down")
	this->m_facing = entity::FACING_DOWN;
else if(key == "walk_left")
	this->m_facing = entity::FACING_LEFT;

		return(this->m_current_animation_iterator);
	}

	const entity::animation_map::const_iterator entity::current_animation(void) const
	{
		if(this->m_current_animation_iterator == this->m_animations->end())
			throw std::logic_error("No entity animation has been started yet. Cannot get current animation.");

		return(this->m_current_animation_iterator);
	}

	const boost::shared_ptr<BITMAP> entity::current_frame(const unsigned int ms) const
	{
		return(this->current_animation()->second->next(ms));
	}

	const entity::facing_direction entity::facing(void) const
	{
		return(this->m_facing);
	}

	const unsigned int entity::h(void) const
	{
		return(this->m_h);
	}

	void entity::load(const std::string &path)
	{
		boost::shared_ptr<TiXmlDocument> doc(new TiXmlDocument(path));
		TiXmlElement *root = NULL;

		if(!doc->LoadFile())
			throw std::runtime_error("Failed to load entity XML document '" + path + "'.");

		if((root = doc->RootElement()) == NULL)
			throw std::runtime_error("Entity XML document '" + path + "' missing root element.");

		this->load(root);
	}

	void entity::load(const TiXmlElement * const entity)
	{
		boost::shared_ptr<BITMAP> bitmap;
		const TiXmlElement *element = NULL;
		const char *str = NULL;

		if((str = entity->Attribute("name")) == NULL)
			throw std::runtime_error("Entity XML missing required name attribute.");

		this->m_name = str;
		entity->Attribute("height", (int *)&this->m_h);
		entity->Attribute("width", (int *)&m_w);
		entity->Attribute("speed", (int *)&m_speed);

		if((element = entity->FirstChildElement("spritesheet")) == NULL)
			throw std::runtime_error("Entity XML entity element missing spritesheet element.");

		this->load_spritesheet(element);

		if(this->m_h == 0 && this->m_w == 0)
		{
			bitmap = this->m_animations->begin()->second->frame(0);

			this->m_h = bitmap->h;
			this->m_w = bitmap->w;
		}
	}

	void entity::load_animation(const unsigned int i, const TiXmlElement * const animation_element, const boost::shared_ptr<iq::spritesheet> sheet, const boost::shared_ptr<unsigned int> anim_w, const boost::shared_ptr<unsigned int> anim_h, const boost::shared_ptr<unsigned int> sheet_ms_per_frame)
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

	const std::string entity::name(void) const
	{
		return(this->m_name);
	}

	const unsigned int entity::screen_x(void) const
	{
		throw std::logic_error("iq::entity::screen_x not implemented yet.");
	}

	const unsigned int entity::screen_y(void) const
	{
		throw std::logic_error("iq::entity::screen_y not implemented yet.");
	}

	const unsigned int entity::speed(void) const
	{
		return(this->m_speed);
	}

	const unsigned int entity::w(void) const
	{
		return(this->m_w);
	}

	const unsigned int entity::x(void) const
	{
		return(this->m_x);
	}

	const unsigned int entity::y(void) const
	{
		return(this->m_y);
	}
}

