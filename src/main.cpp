
#include <main.hpp>

#include <limits.h>

int main(int argc, char *argv[])
{
	boost::shared_ptr<iq::app> app;

	try
	{
		app.reset(new iq::app(argc, argv));
		//g_app = app;
	}
	catch(...)
	{
		exit(-1);
	}

	// Main game loop.
	while(!(key[KEY_ESC] || app->close_button_pressed))
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



//void add_frame(void)
//{
	// Wake up game.
//	sem_post(g_app->sem.get());
//}
//END_OF_FUNCTION(add_frame)



//void close_button_handler(void)
//{
//	g_app->close_button_pressed = true;
//}
//END_OF_FUNCTION(close_button_callback)



void draw(boost::shared_ptr<iq::app> app)
{
/*	textprintf_ex(app->scrbuf.get(), font, 20, 20, WHITE, -1,
			"frame-count: %d",
			g_total_frames);
	textprintf_ex(app->scrbuf.get(), font, 20, 40, WHITE, -1,
			"time: %03d:%02d:%02d:%02d",
			g_days, g_hours,
			g_minutes, g_seconds);
	textprintf_ex(app->scrbuf.get(), font, 20, 60, WHITE, -1,
			"fps: %d",
			g_frames_per_second);
*/

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



//void update_time(void)
//{
//	if(g_seconds == SECS_PER_MIN)
//	{
//		g_minutes++;
//		g_seconds = 0;
//	}
//
//	if(g_minutes == MINS_PER_HOUR)
//	{
//		g_hours++;
//		g_minutes = 0;
//	}
//
//	if(g_hours == HOURS_PER_DAY)
//	{
//		g_days++;
//		g_hours = 0;
//	}
//}

