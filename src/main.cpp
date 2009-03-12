
#include <main.hpp>

int main(int argc, char *argv[])
{
	iq::app_ptr app;

	std::cout.setf(std::ios::unitbuf);

	try
	{
		app.reset(new iq::app(argc, argv));

		// Main game loop.
		while(!(key[KEY_Q] || key[KEY_ESC] || iq::app::close_button_pressed))
		{
			// Sleep until next frame.
			sem_wait(app->sem.get());

			// Logic loop. Changes to the game happen here.
			app->logic();

			/*
			 * Draw. Here we draw the current frame first to a buffer in main
			 * memory and then to the video memory (screen).
			 */
			app->draw();
		}
	}
	catch(std::invalid_argument &ex)
	{
		printf("An invalid argument was passed: %s\n", ex.what());
		allegro_message("An invalid argument was passed: %s\n", ex.what());
		exit(-1);
	}
	catch(std::range_error &ex)
	{
		printf("A range error occurred: %s\n", ex.what());
		allegro_message("A range error occurred: %s\n", ex.what());
		exit(-1);
	}
	catch(std::logic_error &ex)
	{
		printf("A logic error occurred: %s\n", ex.what());
		allegro_message("A logic error occurred: %s\n", ex.what());
		exit(-1);
	}
	catch(std::runtime_error &ex)
	{
		printf("A runtime error occurred: %s\n", ex.what());
		allegro_message("A runtime error occurred: %s\n", ex.what());
		exit(-1);
	}
	catch(std::exception &ex)
	{
		printf("An exception occurred: %s\n", ex.what());
		allegro_message("An exception occurred: %s\n", ex.what());
		exit(-1);
	}

	return(0);
}
END_OF_MAIN()



//void tick(void)
//{
//	g_seconds++;
//	g_frames_per_second = g_frames_this_second;
//	g_frames_this_second = 0;
//}
//END_OF_FUNCTION(tick)

