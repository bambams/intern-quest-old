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


#include <timer.hpp>

namespace iq
{
	const iq::uint timer::HOURS_PER_DAY = 24;
	const iq::uint timer::MINS_PER_HOUR = 60;
	const iq::uint timer::MSECS_PER_SEC = 1000;
	const iq::uint timer::SECS_PER_MIN = 60;
	const iq::uint timer::SECS_PER_HOUR = 60 * 60;
	const iq::uint timer::SECS_PER_DAY = (60 * 60) * 24;

	timer::timer(void)
	{
		this->running = false;
		this->ticks = 0;
	}
	
	iq::uint timer::after_days(const iq::uint total_secs)
	{
		return total_secs - (timer::days(total_secs) * timer::SECS_PER_DAY);
	}
	
	iq::uint timer::after_hours(const iq::uint total_secs)
	{
		return timer::after_days(total_secs) - (timer::hours(total_secs) * timer::SECS_PER_HOUR);
	}
	
	iq::uint timer::after_mins(const iq::uint total_secs)
	{
		return timer::after_hours(total_secs) - (timer::mins(total_secs) * timer::SECS_PER_MIN);
	}
	
	iq::uint timer::days(void) const
	{
		return timer::days(this->total_secs());
	}
	
	iq::uint timer::days(const iq::uint total_secs)
	{
		return total_secs / timer::SECS_PER_DAY;
	}
	
	iq::uint timer::hours(void) const
	{
		return this->hours(this->total_secs());
	}
	
	iq::uint timer::hours(const iq::uint total_secs)
	{
		iq::uint left = timer::after_days(total_secs);
	
		return left / timer::SECS_PER_HOUR;
	}
	
	iq::uint timer::mins(void) const
	{
		return timer::mins(this->total_secs());
	}
	
	iq::uint timer::mins(const iq::uint total_secs)
	{
		iq::uint left = timer::after_hours(total_secs);
	
		return left / timer::SECS_PER_MIN;
	}

	void timer::reset(void)
	{
		this->ticks = 0;
	}

	iq::uint timer::secs(void) const
	{
		return this->after_mins(this->total_secs());
	}

	bool timer::start(void)
	{
		if(!this->running)
			return this->running = (install_param_int_ex(timer::tick, this, BPS_TO_TIMER(1)) == 0);
		return true;
	}

	void timer::stop(void)
	{
		remove_param_int(timer::tick, this);
		this->running = false;
	}

	const std::string timer::to_str(void) const
	{
		iq::uint val[] = {this->days(), this->hours(), this->mins(), this->secs()};
		const char sep = ':';
		const std::streamsize paddedsize = 2;
		std::streamsize strsize;
		std::stringstream ss;

		ss.fill('0');
		strsize = ss.width(3);
		ss << val[0];

		for(int i=1, len=sizeof(val)/sizeof(iq::uint); i<len; i++)
		{
			ss << sep;

			ss.width(paddedsize);
			ss << val[i];

			ss.width(strsize);
		}

		return ss.str();
	}

	float timer::total_days(void) const
	{
		return (float)this->total_secs() / (float)this->SECS_PER_DAY;
	}
	
	float timer::total_hours(void) const
	{
		return (float)this->total_secs() / (float)this->SECS_PER_HOUR;
	}
	
	float timer::total_mins(void) const
	{
		return (float)this->total_secs() / (float)this->SECS_PER_MIN;
	}
	
	iq::uint timer::total_secs(void) const
	{
		return this->ticks;
	}
	
	void timer::tick(void *self)
	{
		IQ_TIMER_TRACE("timer::tick(void *) {");

		((timer *)self)->ticks++;

		IQ_TIMER_TRACE("} //timer::tick(void *)");
	}
	END_OF_FUNCTION(timer::tick)
}

