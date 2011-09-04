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
 
#ifndef base_thread_loop_test_h
#define base_thread_loop_test_h 
 
#include "../base_thread.h"
#include "../../include/hires_timer.h"
#include <sstream>

using namespace nrtb;
using namespace std;

class counter_work_packet: public runnable
{
  public:

	int loop_counter;
	int call_counter;
	int count_to;
	hirez_timer runtime; 
	
	counter_work_packet()
	{
	  loop_counter = 0;
	  call_counter = 0;
	  count_to = 1000;
	  runtime.reset();
	};
	
	~counter_work_packet() {};
	
	void run()
	{
	  runtime.start();
	  call_counter++;
	  for (int i = 0; i < count_to; i++)
	  {
		loop_counter++;
	  };
	  runtime.stop();
	};
	
	string report()
	{
	  stringstream maker;
	  maker << "\ncounter_work_packet Report\n"
		<<   "\tInvoications: " << call_counter 
		<< "\n\tIterations  : " << loop_counter
		<< "\n\tRun Time    : " << runtime.interval()
		<< endl;
	  return maker.str();
	};
	
	string operator() ()
	{
	  return report();
	};
};

#endif // base_thread_loop_test_h