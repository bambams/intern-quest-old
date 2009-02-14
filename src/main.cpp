
#include <main.hpp>

int main(int argc, char *argv[])
{
	boost::shared_ptr<iq::app> app;

	std::cout.setf(std::ios::unitbuf);

	try
	{
		app.reset(new iq::app(argc, argv));
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

