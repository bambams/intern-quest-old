
#ifndef IQ_ENTITY_HPP
	#define IQ_ENTITY_HPP

namespace iq
{
	class entity;
}

	#include <animation.hpp>
	#include <boost/algorithm/string/trim.hpp>
	#include <boost/shared_ptr.hpp>
	#include <map>
	#include <spritesheet.hpp>
	#include <string>
	#include <tinyxml.h>
	#include <vector>

namespace iq
{
	class entity
	{
		typedef std::map<std::string, boost::shared_ptr<iq::animation> > animation_map;
	private:
	protected:
		enum facing_direction { FACING_UP, FACING_RIGHT, FACING_LEFT, FACING_DOWN };
		const static facing_direction DEFAULT_FACING;
		const static unsigned int DEFAULT_SPEED;

		animation_map::const_iterator m_current_animation_iterator;
		boost::shared_ptr<animation_map> m_animations;
		facing_direction m_facing;
		std::string m_name;
		unsigned int m_speed;
		unsigned int m_h, m_w;
		unsigned int m_x, m_y;

		void load_animation(const unsigned int, const TiXmlElement * const, const boost::shared_ptr<iq::spritesheet>, const boost::shared_ptr<unsigned int>, const boost::shared_ptr<unsigned int>, const boost::shared_ptr<unsigned int>);
		void load_spritesheet(const TiXmlElement * const);
	public:
		entity(const std::string &);
		entity(const TiXmlElement * const);
		entity(const std::string &, const unsigned int, const unsigned int);

		const boost::shared_ptr< std::vector<std::string> > animation_keys(void) const;
		const animation_map::const_iterator begin_animation(const std::string &, const unsigned int);
		const boost::shared_ptr<BITMAP> current_frame(const unsigned int) const;
		const animation_map::const_iterator current_animation(void) const;
		const unsigned int h(void) const;
		void load(const std::string &);
		void load(const TiXmlElement * const);
		const std::string name(void) const;
		const unsigned int screen_x(void) const;
		const unsigned int screen_y(void) const;
		const unsigned int speed(void) const;
		const unsigned int w(void) const;

		const unsigned int x(void) const;
		const unsigned int y(void) const;
	};
}

#endif

