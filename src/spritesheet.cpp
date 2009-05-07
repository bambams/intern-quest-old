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


#include <spritesheet.hpp>

namespace iq
{
	spritesheet::spritesheet(const std::string &path, const iq::uint w, const iq::uint h):
		h(h),
		w(w)
	{
		this->load(path, w, h);
	}

	spritesheet::spritesheet(const iq::BITMAP_ptr bitmap, const iq::uint w, const iq::uint h):
		bitmap(bitmap),
		h(h),
		w(w)
	{
	}

	void spritesheet::load(const std::string &path, const iq::uint w, const iq::uint h)
	{
		this->h = h;
		this->w = w;

		this->bitmap.reset(load_bmp(path.c_str(), NULL), destroy_bitmap);

		if(this->bitmap.get() == NULL)
			throw std::runtime_error("Failed to load spritesheet '" + path + "'.");
	}
}

