/***********************************************
 This file is part of the NRTB project (https://*launchpad.net/nrtb).
 
 NRTB is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 NRTB is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with NRTB.  If not, see <http://www.gnu.org/licenses/>.
 
 **********************************************/
#ifndef nrtb_timer_h
#define nrtb_timer_h

#include <sys/time.h>
#include <time.h>
#include <string>

namespace nrtb
{

/** Provides a timer with microsecond (1e-6 second) resolution.
 ** 
 ** Actual resolution on x86 machines is probably closer to 1e-4 or
 ** -5, but it's still pretty good.
 ** 
 ** Usage notes: instanciating this class will start the timer 
 ** automagically. 
 **/
class hirez_timer
{
	public:
		/** initializes the timer and starts it.
		 ** 
		 ** Sets the the timer's elapsed time to init number of seconds and
		 ** then starts the timer.
		 **/
		hirez_timer(const double init = 0);
		/** initalizes the timer and starts it.
		 ** 
		 ** The timer's elapsed time is set to match the value provided in
		 ** init, and the timer is started.
		 **/
		hirez_timer(const timeval & init);

		/** Clears the timer.
		 ** 
		 ** If the timer is running, this stops it as well.
		 **/
		void reset();
		/** Starts the timer.
		 ** 
		 ** Starts the timer, and optionally sets the elapsed time at
		 ** startup to the value provided in init.
		 **/
		void start(const double init = 0);
		/** Starts the timer.
		 ** 
		 ** Sets the elapsed time equal to the value provided in  init
		 ** and then starts the timer.
		 **/
		void start(const timeval & init);
		/** Stops the timer.
		 ** 
		 ** Unconditionally stops the timer, preserving the current
		 ** values. It may be restarted using the start() method, 
		 ** acummulating from where is stopped.
		 ** 
		 ** Returns the elapsed time in seconds.
		 **/
		double stop();
		/** Stops the timer.
		 ** Unconditionally stops the timer, preserving the current
		 ** values. It may be restarted using the start() method, 
		 ** acummulating from where is stopped.
		 ** 
		 ** The return value is the elapsed time in microseconds 
		 ** (1e-6 seconds).
		 **/
		unsigned long long stop_as_usec();
		/** Returns the current elapsed time.
		 ** 
		 ** Returns the number of elapsed seconds on the timer. No change
		 ** is made to the timer's status.
		 **/
		double interval();
		/** Returns the current elapsed time.
		 ** 
		 ** Returns the elapsed time in microseconds (1e-6 seconds). No
		 ** change is made to the timer's status.
		 **/
		unsigned long long interval_as_usec();
		/** Returns the elapsed time in a tm struct.
		 ** 
		 ** See "man gmtime" for the structure of the tm struct. In short
		 ** though, this method provides the current timer value in a
		 ** structure where it's broken down into hours, minutes, seconds and
		 ** the like. No change is made to the timer's status.
		 **/
		 tm interval_as_tm();
		/** Returns the elapsed time in H:MM:SS format
		 **
		 ** Returns the elapsed time in a std::string. The formate is
		 ** H:MM:SS.sssss. No change is made to the timer's status.
		 ** If the optional days parameter is set to true, the number
		 ** of days will be displayed and hours will be < 24, otherwise
		 ** no days are shown and hours will grow as needed (default).
		**/
		std::string interval_as_HMS(const bool days = false);
	private:
		unsigned long long int starttime;
		unsigned long long int elapsed;
		bool running;

		inline double ll_to_double(const unsigned long long int ll);
		inline timeval ll_to_tv(const unsigned long long int ll);
		inline unsigned long long int double_to_ll(const double d);
		inline unsigned long long int tv_to_ll(const timeval & tv);
		inline void stop_timer();
		inline unsigned long long int interval_time();
};

}; // namespace nrtb

#endif // nrtb_timer_h
