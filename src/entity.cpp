
#include <entity.hpp>

namespace iq
{
	const entity::facing_direction entity::DEFAULT_FACING = entity::FACING_DOWN;
	const iq::uint entity::DEFAULT_SPEED = 18;

	entity::entity(const std::string &path):
		m_x(0),
		m_y(0),
		m_animations(new std::map<std::string, iq::animation_ptr>()),
		m_facing(entity::DEFAULT_FACING),
		m_h(0),
		m_w(0)
	{
		this->m_current_animation_iterator = this->m_animations->end();
		this->load(path);
	}

	entity::entity(const TiXmlElement * const entity):
		m_x(0),
		m_y(0),
		m_animations(new std::map<std::string, iq::animation_ptr>()),
		m_facing(entity::DEFAULT_FACING),
		m_h(0),
		m_w(0)
	{
		this->m_current_animation_iterator = this->m_animations->end();
		this->load(entity);
	}

	entity::entity(const std::string &path, const iq::uint w, const iq::uint h):
		m_x(0),
		m_y(0),
		m_animations(new std::map<std::string, iq::animation_ptr>()),
		m_facing(entity::DEFAULT_FACING),
		m_h(h),
		m_w(w)
	{
		this->m_current_animation_iterator = this->m_animations->end();
		this->load(path);
	}

	const boost::shared_ptr<iq::string_vector> entity::animation_keys(void) const
	{
		int j=0;
		boost::shared_ptr<iq::string_vector> keys(new iq::string_vector(this->m_animations->size()));

		for(entity::animation_map::iterator i=this->m_animations->begin(); i != this->m_animations->end(); i++)
			(*keys)[j++] = i->first;

		return keys;
	}

	const entity::animation_map::const_iterator entity::begin_animation(const std::string &key, const iq::uint ms)
	{
		animation_map::const_iterator animation_iterator = this->m_animations->find(key);

		if(animation_iterator == this->m_animations->end())
			throw std::range_error("Invalid animation key '" + key + "'.");

		(this->m_current_animation_iterator = animation_iterator)->second->begin(ms);

		return this->m_current_animation_iterator;
	}

	const entity::animation_map::const_iterator entity::current_animation(void) const
	{
		if(this->m_current_animation_iterator == this->m_animations->end())
			throw std::logic_error("No entity animation has been started yet. Cannot get current animation.");

		return this->m_current_animation_iterator;
	}

	const iq::BITMAP_ptr entity::current_frame(const iq::uint ms) const
	{
		return this->current_animation()->second->next(ms);
	}

	void entity::face(entity::facing_direction facing)
	{
		this->m_facing = facing;
	}

	const entity::facing_direction entity::facing(void) const
	{
		return this->m_facing;
	}

	const iq::uint entity::h(void) const
	{
		return this->m_h;
	}

	void entity::load(const std::string &path)
	{
		iq::TiXmlDocument_ptr doc(new TiXmlDocument(path));
		TiXmlElement *root = NULL;

		if(!doc->LoadFile())
			throw std::runtime_error("Failed to load entity XML document '" + path + "'.");

		if((root = doc->RootElement()) == NULL)
			throw std::runtime_error("Entity XML document '" + path + "' missing root element.");

		this->load(root);
	}

	void entity::load(const TiXmlElement * const entity)
	{
		iq::BITMAP_ptr bitmap;
		const TiXmlElement *element = NULL;
		const char *str = NULL;

		if((str = entity->Attribute("name")) == NULL)
			throw std::runtime_error("Entity XML missing required name attribute.");

		this->m_name = str;
		entity->Attribute("height", (int *)&this->m_h);
		entity->Attribute("width", (int *)&m_w);
		entity->Attribute("speed", (int *)&m_speed);

		if((element = entity->FirstChildElement("animations")) == NULL)
			throw std::runtime_error("Entity XML animations element missing spritesheet element.");

		this->load_animations(element);

		if(this->m_h == 0 && this->m_w == 0)
		{
			bitmap = this->m_animations->begin()->second->frame(0);

			this->m_h = bitmap->h;
			this->m_w = bitmap->w;
		}
	}

	void entity::load_animations(const TiXmlElement * const animations)
	{
		iq::animation_ptr animation;
		iq::uint_ptr default_ms_per_frame(new iq::uint());
		iq::uint_ptr height(new iq::uint());
		iq::uint_ptr width(new iq::uint());
		iq::spritesheet_ptr sheet;
		const char *spritesheet = NULL;
		iq::uint count, length;
		const TiXmlElement *element = NULL;
		const TiXmlNode *node = NULL;
		std::string value;

		if((spritesheet = animations->Attribute("spritesheet")) == NULL)
			throw std::runtime_error("Entity XML animations element missing file attribute.");

		if(animations->Attribute("count", (int *)&count) == NULL)
			throw std::runtime_error("Entity XML animations element missing height attribute.");

		if(animations->Attribute("length", (int *)&length) == NULL)
			throw std::runtime_error("Entity XML animations element missing width attribute.");

		if(animations->Attribute("height", (int *)height.get()) == NULL)
			height.reset((iq::uint *)NULL);

		if(animations->Attribute("width", (int *)width.get()) == NULL)
			width.reset((iq::uint *)NULL);

		if((height.get() == NULL) != (width.get() == NULL))
			throw std::runtime_error("Entity XML spritesheet attributes anim_width and anim_height must either both exist or neither can exist.");

		if(animations->Attribute("ms_per_frame", (int *)default_ms_per_frame.get()) == NULL)
			default_ms_per_frame.reset((iq::uint *)NULL);

		if((element = animations->FirstChildElement("animation")) == NULL)
			throw std::runtime_error("Entity XML spritesheet element missing animation element(s).");

		sheet.reset(new iq::spritesheet(spritesheet, length, count));

		for(iq::uint i=0; i<sheet->h; i++)
		{
			if(element == NULL)
				throw std::runtime_error("Entity XML spritesheet element missing animation element(s).");

			if((node = element->FirstChild()) == NULL || node->Type() != TiXmlNode::TEXT)
				throw std::runtime_error("Entity XML animation node missing key (name) as text node.");

			value = node->ToText()->Value();
			boost::algorithm::trim(value);

			if(value.length() == 0)
				throw std::runtime_error("Entity XML animation node text node (key/name) is empty.");

			animation.reset(new iq::animation(element, sheet, i, width, height, default_ms_per_frame));
			(*this->m_animations)[value] = animation;

			element = element->NextSiblingElement("animation");
		}
	}

	const std::string entity::name(void) const
	{
		return this->m_name;
	}

	void entity::pause_animation(void)
	{
		this->current_animation()->second->pause();
	}

	void entity::reset_animation(void)
	{
		this->current_animation()->second->reset();
	}

	const iq::uint entity::screen_x(void) const
	{
		throw std::logic_error("iq::entity::screen_x not implemented yet.");
	}

	const iq::uint entity::screen_y(void) const
	{
		throw std::logic_error("iq::entity::screen_y not implemented yet.");
	}

	const iq::uint entity::speed(void) const
	{
		return this->m_speed;
	}

	const iq::uint entity::w(void) const
	{
		return this->m_w;
	}

	const iq::uint entity::x(void) const
	{
		return this->m_x;
	}

	const iq::uint entity::y(void) const
	{
		return this->m_y;
	}
}

