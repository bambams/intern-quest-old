
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
		static const unsigned int MINS_PER_HOUR;
		static const unsigned int HOURS_PER_DAY;
		static const unsigned int SECS_PER_MIN;
		static const unsigned int SECS_PER_HOUR;
		static const unsigned int SECS_PER_DAY;

		bool running;
		int ticks;

		timer(void);
		~timer(void);

		unsigned int                days(void) const;
		unsigned int                hours(void) const;
		unsigned int                mins(void) const;
		void                        reset(void);
		unsigned int                secs(void) const;
		bool                        start(void);
		void                        stop(void);
		//boost::shared_ptr<char>     to_str(int &) const;
		std::string                 to_str(void) const;
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

