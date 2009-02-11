
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
		logic(app);

		/*
		 * Draw. Here we draw the current frame first to a buffer in main
		 * memory and then to the video memory (screen).
		 */
		draw(app);
	}

	return(0);
}
END_OF_MAIN()



void draw(boost::shared_ptr<iq::app> app)
{
//	textprintf_ex(app->scrbuf.get(), font, 20, 20, WHITE, -1,
//			"frame-count: %d",
//			g_total_frames);
	textprintf_ex(app->scrbuf.get(), font, 20, 40, WHITE, -1,
			"time: %s", app->timer->to_str().c_str());
//	textprintf_ex(app->scrbuf.get(), font, 20, 60, WHITE, -1,
//			"fps: %d",
//			g_frames_per_second);

	blit(app->scrbuf.get(), screen, 0, 0, 0, 0, 800, 600);
	clear(app->scrbuf.get());
}



void logic(boost::shared_ptr<iq::app> app)
{
/*	update_time();

	g_total_frames++;
	g_frames_this_second++;
*/
}



//void tick(void)
//{
//	g_seconds++;
//	g_frames_per_second = g_frames_this_second;
//	g_frames_this_second = 0;
//}
//END_OF_FUNCTION(tick)

