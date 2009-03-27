
#ifndef MAIN_HPP
	#define MAIN_HPP

	#include <allegro.h>
	#include <animation.hpp>
	#include <app.hpp>
	#include <boost/shared_ptr.hpp>
	#include <cstdio>
	#include <cstdlib>
	#include <ctile.hpp>
	#include <entity.hpp>
	#include <iostream>
	#include <list>
	#include <semaphore.h>
	#include <spritesheet.hpp>
	#include <stdexcept>

	#define BLUE makecol(0, 0, 255)
	#define RED makecol(255, 0, 0)
	#define WHITE makecol(255, 255, 255)
	#define NUM_DIMENSIONS 2

int main(int, char *[]);

#endif

