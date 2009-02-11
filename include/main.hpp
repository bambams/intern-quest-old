
#ifndef MAIN_HPP
	#define MAIN_HPP

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <cstdio>
	#include <cstdlib>
	#include <iostream>
	#include <iq.hpp>
	#include <semaphore.h>

	#define NUM_DIMENSIONS 2
	#define WHITE makecol(255, 255, 255)

//const int SECS_PER_MIN = 60;
//const int MINS_PER_HOUR = 60;
//const int HOURS_PER_DAY = 24;
//const int TARGET_FPS = 30;

//sem_t                          g_semaphore;
//boost::shared_ptr<iq::app> g_app; // TEMPORARY.

//volatile bool                  g_close_button_pressed = false;
//volatile unsigned short int    g_frames_per_second = 0;
//volatile unsigned short int    g_frames_this_second = 0;
//volatile unsigned int          g_total_frames = 0;

//volatile unsigned short int    g_seconds = 0;
//unsigned short int             g_minutes = 0;
//unsigned short int             g_hours = 0;
//unsigned short int             g_days = 0;

//void add_frame(void *);
//void close_button_handler(void);
//void deinitialize(boost::shared_ptr<iq::app> app);
void draw(boost::shared_ptr<iq::app> app);
//int initialize(boost::shared_ptr<iq::app> app);
void logic(boost::shared_ptr<iq::app> app);
int main(int, char *[]);
//void tick(void *);
//void update_time(void);

#endif

