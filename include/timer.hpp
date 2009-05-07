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


#ifndef IQ_TIMER_HPP
	#define IQ_TIMER_HPP

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <cstdio>
	#include <iostream>
	#include <string>
	#include <sstream>

namespace iq
{
	class timer;
	typedef boost::shared_ptr<timer> timer_ptr;
}

	#include <types.hpp>

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
		static iq::uint after_days(const iq::uint);
		static iq::uint after_hours(const iq::uint);
		static iq::uint after_mins(const iq::uint);
	public:
		static const iq::uint HOURS_PER_DAY;
		static const iq::uint MSECS_PER_SEC;
		static const iq::uint MINS_PER_HOUR;
		static const iq::uint SECS_PER_MIN;
		static const iq::uint SECS_PER_HOUR;
		static const iq::uint SECS_PER_DAY;

		bool running;
		int ticks;

		timer(void);

		iq::uint                days(void) const;
		iq::uint                hours(void) const;
		iq::uint                mins(void) const;
		void                    reset(void);
		iq::uint                secs(void) const;
		bool                    start(void);
		void                    stop(void);
		const std::string       to_str(void) const;
		float                   total_days(void) const;
		float                   total_hours(void) const;
		float                   total_mins(void) const;
		iq::uint                total_secs(void) const;

		static iq::uint         days(const iq::uint);
		static iq::uint         hours(const iq::uint);
		static iq::uint         mins(const iq::uint);

		static void             tick(void *);
	};
}

#endif

