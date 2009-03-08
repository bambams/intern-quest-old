
#include <main.hpp>

int main(int argc, char *argv[])
{
	boost::shared_ptr<iq::app> app;

	std::cout.setf(std::ios::unitbuf);

	try
	{
		app.reset(new iq::app(argc, argv));

/*
 * h4x: testing animation.
 */
boost::shared_ptr<BITMAP> bitmap;
boost::shared_ptr<iq::animation> playeranimation;
boost::shared_ptr<iq::spritesheet> playersheet;
int x, y;

playersheet.reset(new iq::spritesheet("media/player.bmp", 3, 4));
playeranimation.reset(new iq::animation(playersheet, 1));

bitmap = playeranimation->begin(275, app->ms);

allegro_message("screen_w=%d screen_h=%d\nplayer_w=%d player_h=%d", app->scrbuf->w, app->scrbuf->h, playeranimation->width(), playeranimation->height());

x = (app->scrbuf->w / 2) - (playeranimation->width() / 2);
y = (app->scrbuf->h / 2) - (playeranimation->height() / 2);

		// Main game loop.
		while(!(key[KEY_Q] || key[KEY_ESC] || iq::app::close_button_pressed))
		{
			// Sleep until next frame.
			sem_wait(app->sem.get());

			// Logic loop. Changes to the game happen here.
			app->logic();

/*
 * h4x: testing animation.
 */
masked_blit(bitmap.get(), app->scrbuf.get(), 0, 0, x, y, bitmap->w, bitmap->h);
bitmap = playeranimation->next(app->ms);

			/*
			 * Draw. Here we draw the current frame first to a buffer in main
			 * memory and then to the video memory (screen).
			 */
			app->draw();
		}
	}
	catch(std::invalid_argument &ex)
	{
		allegro_message("An invalid argument was passed: %s\n", ex.what());
	}
	catch(std::range_error &ex)
	{
		allegro_message("A range error occurred: %s\n", ex.what());
	}
	catch(std::logic_error &ex)
	{
		allegro_message("A logic error occurred: %s\n", ex.what());
	}
	catch(std::runtime_error &ex)
	{
		allegro_message("A runtime error occurred: %s\n", ex.what());
	}
	catch(std::exception &ex)
	{
		allegro_message("An exception occurred: %s\n", ex.what());
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

