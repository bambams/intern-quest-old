
#ifndef IQ_TIMER_HPP
	#define IQ_TIMER_HPP

namespace iq
{
	class timer;
}

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <cstdio>
	#include <iostream>
	#include <string>
	#include <sstream>

	/*
	 * MACRO to output trace information to help debug where the timer class
	 * goes wrong. If it's going wrong. :P To enable, uncomment the
	 * do...while(0), which is required to prevent subtle bugs, and add a
	 * backslash (\) to the end of the define line to continue the MACRO onto
	 * the do...while lines. You can Google for ways to avoid MACRO bugs or
	 * search www.allegro.cc, which is where I picked up the tip.
	 */
	#define IQ_TIMER_TRACE(msg)               
/*		do                                    \
		{                                     \
			std::cout << msg << std::endl;    \
		}while(0)
*/

namespace iq
{
	class timer
	{
	private:
	protected:
		static unsigned int after_days(const unsigned int);
		static unsigned int after_hours(const unsigned int);
		static unsigned int after_mins(const unsigned int);
	public:
		static const unsigned int HOURS_PER_DAY;
		static const unsigned int MSECS_PER_SEC;
		static const unsigned int MINS_PER_HOUR;
		static const unsigned int SECS_PER_MIN;
		static const unsigned int SECS_PER_HOUR;
		static const unsigned int SECS_PER_DAY;

		bool running;
		int ticks;

		timer(void);

		unsigned int                days(void) const;
		unsigned int                hours(void) const;
		unsigned int                mins(void) const;
		void                        reset(void);
		unsigned int                secs(void) const;
		bool                        start(void);
		void                        stop(void);
		const std::string           to_str(void) const;
		float                       total_days(void) const;
		float                       total_hours(void) const;
		float                       total_mins(void) const;
		unsigned int                total_secs(void) const;

		static unsigned int         days(const unsigned int);
		static unsigned int         hours(const unsigned int);
		static unsigned int         mins(const unsigned int);

		static void                 tick(void *);
	};
}

#endif

