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


#include <main.hpp>

int main(int argc, char *argv[])
{
	iq::app_ptr app;

	std::cout.setf(std::ios::unitbuf);

	try
	{
		app.reset(new iq::app("Intern's Quest", argc, argv));

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
		if(screen)
			fprintf(stderr, "An invalid argument was passed: %s\n", ex.what());
		allegro_message("An invalid argument was passed: %s\n", ex.what());
		exit(-1);
	}
	catch(std::range_error &ex)
	{
		if(screen)
			fprintf(stderr, "A range error occurred: %s\n", ex.what());
		allegro_message("A range error occurred: %s\n", ex.what());
		exit(-1);
	}
	catch(std::logic_error &ex)
	{
		if(screen)
			fprintf(stderr, "A logic error occurred: %s\n", ex.what());
		allegro_message("A logic error occurred: %s\n", ex.what());
		exit(-1);
	}
	catch(std::runtime_error &ex)
	{
		if(screen)
			fprintf(stderr, "A runtime error occurred: %s\n", ex.what());
		allegro_message("A runtime error occurred: %s\n", ex.what());
		exit(-1);
	}
	catch(std::exception &ex)
	{
		if(screen)
			fprintf(stderr, "An exception occurred: %s\n", ex.what());
		allegro_message("An exception occurred: %s\n", ex.what());
		exit(-1);
	}

	return 0;
}
END_OF_MAIN()



//void tick(void)
//{
//	g_seconds++;
//	g_frames_per_second = g_frames_this_second;
//	g_frames_this_second = 0;
//}
//END_OF_FUNCTION(tick)

