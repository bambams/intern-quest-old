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

#ifndef TILE_HPP
	#define TILE_HPP

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <vector>

namespace iq
{
	class tile;
	typedef boost::shared_ptr<tile> tile_ptr;
	typedef std::vector<tile> tile_vector;
}

	#include <types.hpp>

namespace iq
{
	class tile
	{
		protected:
		public:
			iq::BITMAP_ptr bitmap;

			tile(const iq::BITMAP_ptr);
	};
}

#endif

