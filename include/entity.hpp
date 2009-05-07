/*
 * Intern's Quest is an adventure game.
 * Copyright (C) 2009 Brandon McCaig, Samuel Henderson
 *
 * This file is part of Intern's Quest.
 *
 * Intern's Quest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Intern's Quest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Intern's Quest.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef IQ_ENTITY_HPP
	#define IQ_ENTITY_HPP

	#include <boost/algorithm/string/trim.hpp>
	#include <boost/shared_ptr.hpp>
	#include <map>
	#include <string>
	#include <tinyxml.h>
	#include <vector>

namespace iq
{
	class entity;
	typedef boost::shared_ptr<entity> entity_ptr;
}

	#include <animation.hpp>
	#include <entity_y_comparer.hpp>
	#include <spritesheet.hpp>
	#include <tilemap.hpp>
	#include <types.hpp>

namespace iq
{
	class entity
	{
	private:
	protected:
		typedef std::map<std::string, boost::shared_ptr<iq::animation> > animation_map;
	public:
		enum facing_direction { FACING_UP, FACING_RIGHT, FACING_LEFT, FACING_DOWN };
		typedef iq::entity_y_comparer y_comparer;

		entity(const std::string &);
		entity(const TiXmlElement * const);
		entity(const std::string &, const iq::uint, const iq::uint);

		const iq::string_vector_ptr animation_keys(void) const;
		const std::map<std::string, iq::animation_ptr>::const_iterator begin_animation(const std::string &, const iq::uint);
		const iq::BITMAP_ptr current_frame(const iq::uint) const;
		const std::map<std::string, iq::animation_ptr>::const_iterator current_animation(void) const;
		void face(facing_direction);
		const facing_direction facing(void) const;
		const iq::uint h(void) const;
		void load(const std::string &);
		void load(const TiXmlElement * const);
		const std::string name(void) const;
		void pause_animation(void);
		void reset_animation(void);
		const int screen_x(const iq::BITMAP_ptr, const iq::entity_ptr) const;
		const int screen_y(const iq::BITMAP_ptr, const iq::entity_ptr) const;
		const iq::uint speedx(void) const;
		const iq::uint speedy(void) const;
		const iq::uint w(void) const;
		const int x(void) const;
		const int y(void) const;

		int m_x, m_y;
	protected:
		const static facing_direction DEFAULT_FACING;
		const static iq::uint DEFAULT_SPEED;

		std::map<std::string, iq::animation_ptr>::const_iterator m_current_animation_iterator;
		boost::shared_ptr< std::map<std::string, iq::animation_ptr> > m_animations;
		facing_direction m_facing;
		std::string m_name;
		iq::uint m_speedx;
		iq::uint m_speedy;
		iq::uint m_h, m_w;

		void load_animation(const iq::uint, const TiXmlElement * const, const iq::spritesheet_ptr, const iq::uint_ptr, const iq::uint_ptr, const iq::uint_ptr);
		void load_animations(const TiXmlElement * const);
	private:
	};
}

#endif

