
#include <main.hpp>

int main(int argc, char *argv[])
{
	long long i = 0;
	unsigned int last_secs = -1;
	boost::shared_ptr<BITMAP> scrbuf;
	//boost::shared_ptr<char> t;
	boost::shared_ptr<iq::timer> timer;

	std::cout.setf(std::ios::unitbuf);

	try
	{
		initialize(scrbuf);
	}
	catch(int ex)
	{
		std::cout << "Exception of type `int' caught in main(). Value '" << ex << "'. Aborting..." << std::endl;
		exit(-1);
	}
	catch(...)
	{
		std::cout << "Unknown exception caught in main(). Aborting..." << std::endl;
		exit(-1);
	}

	try
	{
		timer.reset(new iq::timer);
	}
	catch(std::bad_alloc ex)
	{
		std::cout << "Failed to instantiate timer in main(). Aborting..." << std::endl;
		exit(-1);
	}

	timer->start();

	while(!(key[KEY_Q] || key[KEY_ESC] || g_close_button_pressed))
	{
		rest(1);

		// Reset timer.
		if(key[KEY_R] && i == 0)
			timer->reset();

		// Stop timer.
		if(key[KEY_S])
			if(timer->running)
				timer->stop();

		// Start timer (AKA Go).
		if(key[KEY_G])
			if(!timer->running)
				timer->start();

		// Draw.
		if(last_secs != timer->total_secs())
		{
			textprintf_ex(scrbuf.get(), font, 20, 20, WHITE, -1, "Start/Resume Timer (Go): G    Reset Timer: R    Stop Timer: S");
			textprintf_ex(scrbuf.get(), font, 20, 30, WHITE, -1, "Quit: ESC or Q");
			textprintf_ex(scrbuf.get(), font, 20, 40, WHITE, -1, "Time: %s", timer->to_str().c_str());
			blit(scrbuf.get(), screen, 0, 0, 0, 0, screen->w, screen->h);
			last_secs = timer->total_secs();
			clear(scrbuf.get());
		}
	}

	printf("\n");

	return(0);
}



void close_button_handler(void)
{
	g_close_button_pressed = true;
}



void initialize(boost::shared_ptr<BITMAP> &scrbuf)
{
//	int i, len;
	const char* msg = "If at first you don't succeed,...you fail";
/*	const int X = 0, Y = 1;
	int r[][NUM_DIMENSIONS] = {
	                              {1024, 768},
	                              {800, 600},
	                              {640, 480}
	                          };
*/
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

/*	for(i=0, len=sizeof(r)/sizeof(int)/NUM_DIMENSIONS; i<len; i++)
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
*/
	if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) != 0)
	{
		printf("%s [to set the graphics mode]. %s.\n",
				msg, allegro_error);
		throw(-1);
	}

	scrbuf.reset(create_bitmap(640, 480), destroy_bitmap);

	if(scrbuf.get() == NULL)
	{
		printf("%s [to create a screen buffer].\n", msg);
		throw(-1);
	}

	clear(scrbuf.get());

	if(show_os_cursor(MOUSE_CURSOR_ARROW) < 0)
	{
		printf("%s [to show the operating system mouse cursor].\n", msg);
	}

	if(set_display_switch_mode(SWITCH_BACKGROUND) != 0)
	{
		printf("%s [to set display switch mode]. %s.\n",
				msg, allegro_error);
		throw(-1);
	}

	LOCK_VARIABLE(g_close_button_pressed);
	LOCK_FUNCTION(close_button_hander);

	set_close_button_callback(close_button_handler);
}

