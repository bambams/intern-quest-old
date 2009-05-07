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


#ifndef IQ_APP_HPP
	#define IQ_APP_HPP

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <iostream>
	#include <semaphore.h>
	#include <stdexcept>
	#include <string>
	#include <tinyxml.h>

namespace iq
{
	class app;
	typedef boost::shared_ptr<app> app_ptr;
}

	#include <entity.hpp>
	#include <timer.hpp>
	#include <map>
	#include <select_value.hpp>
	#include <tilemap.hpp>

	#ifdef IQ_APP_TRACE
		#error OMFG, IQ_APP_TRACE already exists! \o/
	#endif

	/*
	 * MACRO to output trace information to help debug where the app class goes
	 * wrong. If it's going wrong. :P To enable, uncomment the do...while(0),
	 * which is required to prevent subtle bugs, and add a backslash (\) to
	 * the end of the define line to continue the MACRO onto the do...while
	 * lines. You can Google for ways to avoid MACRO bugs or search
	 * www.allegro.cc, which is where I picked up the tip.
	 */
	#define IQ_APP_TRACE(msg)                 
/*		do                                    \
		{                                     \
			std::cout << msg << std::endl;    \
		}while(0)
*/

	#define BLUE makecol(0, 0, 255)
	#define LIGHTGREEN makecol(155, 255, 155)
	#define NUM_DIMENSIONS 2
	#define RED makecol(255, 0, 0)
	#define WHITE makecol(255, 255, 255)

namespace iq
{
	/**
	 * \brief An application class to hold application state data.
	 * \details Intended to be passed around the application to avoid
	 *          global state. Any sort of data that needs to be available
	 *          to the application that comes from far away can be attached
	 *          to this class... >_> A first-attempt at avoiding global
	 *          state
	 */
	class app
	{
	private:
	protected:
		// Method pointer for draw and logic.
		typedef void (app::*void_method_void)(void);

		const static std::string DEFAULT_FILE;

		std::string m_file;

		void_method_void m_drawptr;
		void_method_void m_logicptr;

		void load_entities(const TiXmlElement * const);
	public:
		// `state' wouldn't work for a typename and variable name so gamestate is what it became...
		enum gamestate {SETUP, GAMEPLAY, SCRIPTED, CREDITS};

		iq::string_map argv;
		std::map<std::string, iq::entity_ptr> entities;
		std::vector<iq::entity_ptr> drawn_entities;
		iq::uint fts; // frames this second.
		iq::tilemap_ptr map;
		iq::uint ms;
		bool os_cursor;
		iq::entity_ptr player;
		iq::BITMAP_ptr scrbuf;
		iq::sem_t_ptr sem;
		gamestate state;
		iq::uint target_fps;
		std::map<std::string, iq::tile_ptr> tiles;
		iq::timer_ptr timer;
		iq::uint total_frames;
		bool verbose;

		static bool close_button_pressed;

		app(const char * const, int, char *[]);
		~app(void);

		void deinitialize(void);
		void draw(void);
		void draw_setup(void);
		void draw_gameplay(void);
		void draw_scripted(void);
		void draw_credits(void);
		void initialize(void);
		void load(void);
		void load(const TiXmlElement * const);
		void logic(void);
		void logic_setup(void);
		void logic_gameplay(void);
		bool horizontal_collision(int x, int y, int w, int &tilecoordy); //hax
		bool vertical_collision(int x, int y, int h, int &tilecoordx); //hax

		void logic_scripted(void);
		void logic_credits(void);
		void parse_args(int, char *[]);
		void set_state(gamestate);

		static void add_frame(void *);
		static void close_button_handler(void);
		static void sem_destroy(sem_t *);
	};
}

#endif

