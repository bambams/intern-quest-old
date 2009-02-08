
#include <iq/timer.hpp>

namespace iq
{
	const int timer::HOURS_PER_DAY = 24;
	const int timer::MINS_PER_HOUR = 60;
	const int timer::SECS_PER_MIN = 60;
	const int timer::SECS_PER_HOUR = 60 * 60;
	const int timer::SECS_PER_DAY = (60 * 60) * 24;
	
	timer::timer(void)
	{
		this->ticks = 0;
	
		install_param_int_ex(timer::tick, this, BPS_TO_TIMER(1));
	}
	
	timer::~timer(void)
	{
		remove_param_int(timer::tick, this);
	}
	
	int timer::after_days(const int total_secs)
	{
		return(total_secs - (timer::days(total_secs) * SECS_PER_DAY));
	}
	
	int timer::after_hours(const int total_secs)
	{
		return(timer::after_days(total_secs) - (timer::hours(total_secs) * SECS_PER_HOUR));
	}
	
	int timer::after_mins(const int total_secs)
	{
		return(timer::after_hours(total_secs) - (timer::mins(total_secs) * SECS_PER_MIN));
	}
	
	int timer::days(void) const
	{
		return(timer::days(this->total_secs()));
	}
	
	int timer::days(const int total_secs)
	{
		return(total_secs % SECS_PER_DAY);
	}
	
	int timer::hours(void) const
	{
		return(timer::hours(this->total_secs()));
	}
	
	int timer::hours(const int total_secs)
	{
		int left = timer::after_days(total_secs);
	
		return(left - left % SECS_PER_HOUR);
	}
	
	int timer::mins(void) const
	{
		return(timer::mins(timer::total_secs()));
	}
	
	int timer::mins(const int total_secs)
	{
		int left = timer::after_hours(total_secs);
	
		return(left - left % SECS_PER_MIN);
	}
	
	int timer::secs(void) const
	{
		return(this->after_mins(this->total_secs()));
	}
	
	float timer::total_days(void) const
	{
		return((float)this->total_secs() / (float)this->SECS_PER_DAY);
	}
	
	float timer::total_hours(void) const
	{
		return((float)this->total_secs() / (float)this->SECS_PER_HOUR);
	}
	
	float timer::total_mins(void) const
	{
		return((float)this->total_secs() / (float)this->SECS_PER_MIN);
	}
	
	int timer::total_secs(void) const
	{
		return(this->ticks);
	}
	
	void timer::tick(void *self)
	{
		((timer *)self)->ticks++;
	}
	END_OF_FUNCTION(timer::tick)
}

