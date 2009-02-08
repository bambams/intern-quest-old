
#ifndef IQ_APP_HPP
	#define IQ_APP_HPP

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <iostream>
	#include <map>
	#include <semaphore.h>
	#include <string>

	#define NUM_DIMENSIONS 2

namespace iq
{
	/*
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
	public:
		std::map<std::string, std::string> argv;
		bool close_button_pressed;
		bool os_cursor;
		boost::shared_ptr<BITMAP> scrbuf;
		boost::shared_ptr<sem_t> sem;
		int target_fps;

		app(int, char *[]);
		~app(void);

		void deinitialize(void);
		void initialize(void);
		void parse_args(int argc, char *[]);

		static void add_frame(void *);
		static void close_button_handler(void *);
	};
}

#endif

