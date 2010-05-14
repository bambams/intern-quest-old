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


#ifndef IQ_TYPES_HPP
	#define IQ_TYPES_HPP

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <map>
	#include <semaphore.h>
	#include <tinyxml.h>
	#include <vector>

namespace iq
{
	typedef unsigned int uint;

	typedef std::map<std::string, std::string> string_map;
	typedef std::vector<std::string> string_vector;

	typedef boost::shared_ptr<BITMAP> BITMAP_ptr;
	typedef boost::shared_ptr<TiXmlDocument> TiXmlDocument_ptr;
	typedef boost::shared_ptr<sem_t> sem_t_ptr;
	typedef boost::shared_ptr<string_map> string_map_ptr;
	typedef boost::shared_ptr<string_vector> string_vector_ptr;
	typedef boost::shared_ptr<uint> uint_ptr;
}

#endif

