
#ifndef MAIN_HPP
	#define MAIN_HPP

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <iostream>
	#include <iq/timer.hpp>
	#include <stdio.h>

	#define WHITE makecol(255, 255, 255)

bool g_close_button_pressed;

void close_button_handler(void);
int main(int, char *[]);
void initialize(boost::shared_ptr<BITMAP> &);

#endif

