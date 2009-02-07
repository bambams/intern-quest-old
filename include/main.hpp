
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

const int SECS_PER_MIN = 60;
const int MINS_PER_HOUR = 60;
const int HOURS_PER_DAY = 24;
const int TARGET_FPS = 30;

sem_t                          g_semaphore;

volatile bool                  g_close_button_pressed = false;
volatile unsigned short int    g_frames_per_second = 0;
volatile unsigned short int    g_frames_this_second = 0;
volatile unsigned int          g_total_frames = 0;

volatile unsigned short int    g_seconds = 0;
unsigned short int             g_minutes = 0;
unsigned short int             g_hours = 0;
unsigned short int             g_days = 0;

void add_frame(void);
void close_button_handler(void);
void draw(iq::app &, boost::shared_ptr<BITMAP>);
int initialize(iq::app &);
void logic(iq::app &);
int main(int, char *[]);
void tick(void);
void update_time(void);

#endif

