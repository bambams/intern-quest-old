
#ifndef IQ_APP_HPP
	#define IQ_APP_HPP

namespace iq
{
	class app;
}

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <entity.hpp>
	#include <iostream>
	#include <timer.hpp>
	#include <map>
	#include <semaphore.h>
	#include <stdexcept>
	#include <string>
	#include <tilemap.hpp>
	#include <tinyxml.h>

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

		std::map<std::string, std::string> argv;
//		boost::shared_ptr<iq::tilemap> demo_map;
		std::map<std::string, boost::shared_ptr<iq::entity> > entities;
		unsigned int fts; // frames this second.
		unsigned int ms;
		bool os_cursor;
		boost::shared_ptr<iq::entity> player;
		boost::shared_ptr<BITMAP> scrbuf;
		boost::shared_ptr<sem_t> sem;
		gamestate state;
		unsigned int target_fps;
		boost::shared_ptr<iq::timer> timer;
		unsigned int total_frames;
		bool verbose;

		static bool close_button_pressed;

		app(int, char *[]);
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
		void logic_scripted(void);
		void logic_credits(void);
		void parse_args(int, char *[]);
		void set_state(gamestate);

		static void add_frame(void *);
		//static void close_button_handler(void *);
		static void close_button_handler(void);
		static void sem_destroy(sem_t *);
	};
}

#endif

