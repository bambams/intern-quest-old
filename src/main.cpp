
#include <main.hpp>

int main(int argc, char *argv[])
{
	boost::shared_ptr<BITMAP> b;

	// Try to initialize game.
	if(initialize() != 0)
		exit(-1);

	// Initalize semaphore.
	sem_init(&g_semaphore, 0, 1);

	// Create and clear screen buffer.
	b.reset(create_bitmap(800, 600), destroy_bitmap);
	clear(b.get());

	// Main game loop.
	while(!(key[KEY_ESC] || g_close_button_pressed))
	{
		// Sleep until next frame.
		sem_wait(&g_semaphore);

		// Logic loop. Changes to the game happen here.
		logic();

		/*
		 * Draw. Here we draw the current frame first to a buffer in main memory and
		 * then to the video memory (screen).
		 */
		draw(b);
	}

	return(0);
}
END_OF_MAIN()



void add_frame(void)
{
	// Wake up game.
	sem_post(&g_semaphore);
}
END_OF_FUNCTION(add_frame)



void close_button_handler(void)
{
	g_close_button_pressed = true;
}
END_OF_FUNCTION(close_button_callback)



void draw(boost::shared_ptr<BITMAP> b)
{
	textprintf_ex(b.get(), font, 20, 20, WHITE, -1, "frame-count: %d", g_total_frames);
	textprintf_ex(b.get(), font, 20, 40, WHITE, -1, "time: %03d:%02d:%02d:%02d", g_days, g_hours, g_minutes, g_seconds);
	textprintf_ex(b.get(), font, 20, 60, WHITE, -1, "fps: %d", g_frames_per_second);

	blit(b.get(), screen, 0, 0, 0, 0, 800, 600);
	clear(b.get());
}



int initialize(void)
{
	const char* msg = "If at first you don't succeed,...you fail";

	if(allegro_init() != 0)
	{
		printf("%s [to initialize Allegro]. %s.\n", msg, allegro_error);
		return(-1);
	}

	if(install_keyboard() != 0)
	{
		printf("%s [to install the keyboard routines].\n", msg);
		return(-1);
	}

	if(install_timer() != 0)
	{
		printf("%s [to install the timer routines].", msg);
		return(-1);
	}

	set_color_depth(32);

	if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0) != 0)
	{
		printf("%s [to set the graphics mode]. %s.\n", msg, allegro_error);
		return(-1);
	}

	if(set_display_switch_mode(SWITCH_BACKGROUND) != 0)
	{
		printf("%s [to set display switch mode].\n", msg, allegro_error);
		return(-1);
	}

	LOCK_VARIABLE(g_close_button_pressed);
	LOCK_VARIABLE(g_pending_frames);
	LOCK_VARIABLE(g_frames_per_second);
	LOCK_VARIABLE(g_frames_this_second);
	LOCK_VARIABLE(g_seconds);
	LOCK_VARIABLE(g_minutes);
	LOCK_VARIABLE(g_hours);
	LOCK_VARIABLE(g_days);

	LOCK_FUNCTION(add_frame);
	LOCK_FUNCTION(close_button_handler);
	LOCK_FUNCTION(tick);

	install_int_ex(add_frame, BPS_TO_TIMER(TARGET_FPS));
	install_int_ex(tick, BPS_TO_TIMER(1));

	set_close_button_callback(close_button_handler);

	return(0);
}



void logic(void)
{
	update_time();

	g_total_frames++;
	g_frames_this_second++;
}



void tick(void)
{
	g_seconds++;
	g_frames_per_second = g_frames_this_second;
	g_frames_this_second = 0;
}
END_OF_FUNCTION(tick)



void update_time(void)
{
	if(g_seconds == SECS_PER_MIN)
	{
		g_minutes++;
		g_seconds = 0;
	}

	if(g_minutes == MINS_PER_HOUR)
	{
		g_hours++;
		g_minutes = 0;
	}

	if(g_hours == HOURS_PER_DAY)
	{
		g_days++;
		g_hours = 0;
	}
}

