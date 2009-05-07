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


#ifndef IQ_ANIMATION_HPP
	#define IQ_ANIMATION_HPP

	#include <boost/shared_ptr.hpp>
	#include <stdexcept>
	#include <tinyxml.h>
	#include <vector>

namespace iq
{
	class animation;
	typedef boost::shared_ptr<animation> animation_ptr;
}

	#include <spritesheet.hpp>
	#include <types.hpp>

namespace iq
{
	class animation
	{
	private:
	protected:
		std::vector<iq::BITMAP_ptr> m_frames;
		iq::uint m_last_ms;
		iq::uint m_last_frame;
		iq::uint m_ms_per_frame;
		bool m_playing;

		void check_frame_index(const iq::uint) const;
		void check_ms(const iq::uint) const;
		void check_ms_per_frame(void) const;
		void set_last_frame(const iq::uint);
		void set_last_ms(const iq::uint);
		void set_ms_per_frame(const iq::uint);
	public:
		animation(const spritesheet_ptr, const iq::uint);
		animation(const spritesheet_ptr, const iq::uint, const iq::uint);
		animation(const spritesheet_ptr, const iq::uint, const iq::uint, const iq::uint);
		animation(const spritesheet_ptr, const iq::uint, const iq::uint, const iq::uint, const iq::uint);
		animation(const TiXmlElement *, iq::spritesheet_ptr, iq::uint, iq::uint_ptr, iq::uint_ptr, iq::uint_ptr);

		const iq::BITMAP_ptr begin(const iq::uint);
		const iq::BITMAP_ptr begin(const iq::uint, const iq::uint);
		const iq::BITMAP_ptr frame(const iq::uint) const;
		const iq::uint h(void) const;
		void load(const spritesheet_ptr, const iq::uint);
		void load(const spritesheet_ptr, const iq::uint, const iq::uint);
		void load(const spritesheet_ptr, const iq::uint, const iq::uint, const iq::uint);
		void load(const spritesheet_ptr, const iq::uint, const iq::uint, const iq::uint, const iq::uint);
		void load(const TiXmlElement*, iq::spritesheet_ptr, iq::uint, iq::uint_ptr, iq::uint_ptr, iq::uint_ptr);
		const iq::BITMAP_ptr next(const iq::uint);
		void pause(void);
		const bool playing(void) const;
		void reset(void);
		const iq::uint w(void) const;

		const iq::BITMAP_ptr operator[](const iq::uint) const;
	};
}

#endif

