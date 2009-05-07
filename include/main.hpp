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


#ifndef MAIN_HPP
	#define MAIN_HPP

	#include <allegro.h>
	#include <animation.hpp>
	#include <app.hpp>
	#include <boost/shared_ptr.hpp>
	#include <cstdio>
	#include <cstdlib>
	#include <ctile.hpp>
	#include <entity.hpp>
	#include <iostream>
	#include <list>
	#include <semaphore.h>
	#include <spritesheet.hpp>
	#include <stdexcept>

	#define BLUE makecol(0, 0, 255)
	#define RED makecol(255, 0, 0)
	#define WHITE makecol(255, 255, 255)
	#define NUM_DIMENSIONS 2

int main(int, char *[]);

#endif

