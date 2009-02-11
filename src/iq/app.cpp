
#include <iq/app.hpp>

namespace iq
{
	bool app::close_button_pressed = false;

	app::app(int argc, char *argv[])
	{
		IQ_APP_TRACE("iq::app::app(int, char *[]) {");

		this->sem.reset(new sem_t(), app::sem_destroy);
		this->target_fps = 30;
		this->verbose = false;

		this->parse_args(argc, argv);
		this->initialize();

		this->timer.reset(new iq::timer());

		IQ_APP_TRACE("} //iq::app::app(int, char *[])");
	}

	app::~app(void)
	{
		IQ_APP_TRACE("iq::app::~app() {");

		this->deinitialize();

		IQ_APP_TRACE("} //iq::app::~app()");
	}

	void app::add_frame(void *self)
	{
		IQ_APP_TRACE("iq::app::add_frame(void) {");

		sem_post(((iq::app *)self)->sem.get());

		IQ_APP_TRACE("} //iq::app::add_frame(void)");
	}
	END_OF_FUNCTION(app::add_frame)

	void app::close_button_handler(void /*void *self*/)
	{
		IQ_APP_TRACE("iq::app::close_button_handler(void) {");

		iq::app::close_button_pressed = true;

		IQ_APP_TRACE("} //iq::app::close_button_handler(void)");
	}
	END_OF_FUNCTION(app::close_button_handler)

	void app::deinitialize(void)
	{
		IQ_APP_TRACE("iq::app::deinitialize() {");

		remove_param_int(app::add_frame, this);

		IQ_APP_TRACE("} //iq::app::deinitialize()");
	}

	void app::initialize(void)
	{
		IQ_APP_TRACE("iq::app::initialize(void) {");

		int i, len;
		const char* msg = "If at first you don't succeed,...you fail";
		const int X = 0, Y = 1;
		int r[][NUM_DIMENSIONS] = {
		                              {1024, 768},
		                              {800, 600},
		                              {640, 480}
		                          };

		if(allegro_init() != 0)
		{
			printf("%s [to initialize Allegro]. %s.\n", msg, allegro_error);
			throw(-1);
		}

		if(install_keyboard() != 0)
		{
			printf("%s [to install the keyboard routines].\n", msg);
			throw(-1);
		}

		if(install_mouse() < 0)
		{
			printf("%s [to install the mouse routines]. %s.\n",
					msg, allegro_error);
			throw(-1);
		}

		enable_hardware_cursor();

		if(install_timer() != 0)
		{
			printf("%s [to install the timer routines].", msg);
			throw(-1);
		}

		set_color_depth(32);

		for(i=0, len=sizeof(r)/sizeof(int)/NUM_DIMENSIONS; i<len; i++)
		{
			if(set_gfx_mode(GFX_AUTODETECT_WINDOWED,
					r[i][X], r[i][Y], 0, 0) == 0)
			{
				break;
			}
		}

		if(i == len)
		{
			for(i=0; i<len; i++)
				if(set_gfx_mode(GFX_AUTODETECT, r[i][X], r[i][Y], 0, 0) == 0)
					break;

			if(i == len)
			{
				printf("%s [to set the graphics mode]. %s.\n",
						msg, allegro_error);
				throw(-1);
			}
		}

		this->scrbuf.reset(create_bitmap(800, 600), destroy_bitmap);

		if(this->scrbuf.get() == NULL)
		{
			printf("%s [to create a screen buffer].\n", msg);
			throw(-1);
		}

		clear(this->scrbuf.get());

		if(!(this->os_cursor = !show_os_cursor(MOUSE_CURSOR_ARROW)))
		{
			printf("%s [to show the operating system mouse cursor].\n", msg);
		}

		if(set_display_switch_mode(SWITCH_BACKGROUND) != 0)
		{
			printf("%s [to set display switch mode]. %s.\n",
					msg, allegro_error);
			throw(-1);
		}

		//LOCK_VARIABLE(g_close_button_pressed);
		LOCK_VARIABLE(this);
		//LOCK_VARIABLE(g_pending_frames);
		//LOCK_VARIABLE(g_frames_per_second);
		//LOCK_VARIABLE(g_frames_this_second);
		//LOCK_VARIABLE(g_seconds);
		//LOCK_VARIABLE(g_minutes);
		//LOCK_VARIABLE(g_hours);
		//LOCK_VARIABLE(g_days);

		LOCK_FUNCTION(app::add_frame);
		LOCK_FUNCTION(app::close_button_handler);
		//LOCK_FUNCTION(tick);

		if(install_param_int_ex(app::add_frame, this, BPS_TO_TIMER(this->target_fps)) != 0)
		{
			printf("%s [to install the add_frame timer].\n", msg);
			throw(-1);
		}

		set_close_button_callback(app::close_button_handler);

		sem_init(this->sem.get(), 0, 1);

		IQ_APP_TRACE("} //iq::app::initialize(void)");
	}

	void app::parse_args(int argc, char *argv[])
	{
		IQ_APP_TRACE("iq::app::parse_args(int, char *[]) {");

		IQ_APP_TRACE("} //iq::app::parse_args(int, char *[])");
	}

	void app::sem_destroy(sem_t *sem)
	{
		IQ_APP_TRACE("iq::app::sem_destroy(sem_t) {");

		if(sem)
		{
			/*
			 * This call was the cause of a fun bug. Instance scope versus
			 * global scope. :P The result was infinite recursion leading up
			 * to a segfault. I should have known better when I named it. I
			 * proabably should heed the lesson and name it something less
			 * mistakable, but I have lessons to learn and I intend to learn
			 * them. ^_^
			 */
			::sem_destroy(sem);
			delete sem;
		}

		IQ_APP_TRACE("} //iq::app::sem_destroy(sem_t)");
	}
}

