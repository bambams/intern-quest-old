
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
	#include <types.hpp>
	#include <vector>

namespace iq
{
	typedef boost::shared_ptr<entity> entity_ptr;

	class entity
	{
	private:
	protected:
		typedef std::map<std::string, boost::shared_ptr<iq::animation> > animation_map;
	public:
		enum facing_direction { FACING_UP, FACING_RIGHT, FACING_LEFT, FACING_DOWN };

		entity(const std::string &);
		entity(const TiXmlElement * const);
		entity(const std::string &, const iq::uint, const iq::uint);

		const iq::string_vector_ptr animation_keys(void) const;
		const std::map<std::string, iq::animation_ptr>::const_iterator begin_animation(const std::string &, const iq::uint);
		const iq::BITMAP_ptr current_frame(const iq::uint) const;
		const std::map<std::string, iq::animation_ptr>::const_iterator current_animation(void) const;
		const facing_direction facing(void) const;
		const iq::uint h(void) const;
		void load(const std::string &);
		void load(const TiXmlElement * const);
		const std::string name(void) const;
		void pause_animation(void);
		void reset_animation(void);
		const iq::uint screen_x(void) const;
		const iq::uint screen_y(void) const;
		const iq::uint speed(void) const;
		const iq::uint w(void) const;
		const iq::uint x(void) const;
		const iq::uint y(void) const;
	protected:
		const static facing_direction DEFAULT_FACING;
		const static iq::uint DEFAULT_SPEED;

		std::map<std::string, iq::animation_ptr>::const_iterator m_current_animation_iterator;
		boost::shared_ptr< std::map<std::string, iq::animation_ptr> > m_animations;
		facing_direction m_facing;
		std::string m_name;
		iq::uint m_speed;
		iq::uint m_h, m_w;
		iq::uint m_x, m_y;

		void load_animation(const iq::uint, const TiXmlElement * const, const iq::spritesheet_ptr, const iq::uint_ptr, const iq::uint_ptr, const iq::uint_ptr);
		void load_animations(const TiXmlElement * const);
	private:
	};
}

#endif

