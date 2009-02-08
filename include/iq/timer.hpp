
#ifndef IQ_TIMER_HPP
	#define IQ_TIMER_HPP

	#include <allegro.h>

namespace iq
{
	class timer
	{
	private:
	protected:
		static int after_days(const int);
		static int after_hours(const int);
		static int after_mins(const int);
	public:
		static const int MINS_PER_HOUR;
		static const int HOURS_PER_DAY;
		static const int SECS_PER_MIN;
		static const int SECS_PER_HOUR;
		static const int SECS_PER_DAY;

		int ticks;

		timer(void);
		~timer(void);

		int days(void) const;
		int hours(void) const;
		int mins(void) const;
		int secs(void) const;

		static int days(const int);
		static int hours(const int);
		static int mins(const int);

		float total_days(void) const;
		float total_hours(void) const;
		float total_mins(void) const;
		int total_secs(void) const;

		static void tick(void *);
	};
}

#endif

