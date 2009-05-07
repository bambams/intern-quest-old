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


#ifndef IQ_SPRITESHEET_HPP
	#define IQ_SPRITESHEET_HPP

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <stdexcept>
	#include <string>

namespace iq
{
	class spritesheet;
	typedef boost::shared_ptr<spritesheet> spritesheet_ptr;
}

	#include <types.hpp>

namespace iq
{
	class spritesheet
	{
	private:
	protected:
	public:
		spritesheet(const std::string &, const iq::uint, const iq::uint);
		spritesheet(const iq::BITMAP_ptr, const iq::uint, const iq::uint);

		iq::BITMAP_ptr bitmap;
		iq::uint h;
		iq::uint w;

		void load(const std::string &, const iq::uint, const iq::uint);
	};
}

#endif

