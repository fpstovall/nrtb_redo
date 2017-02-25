/***********************************************
 This file is part of the NRTB project (https://github.com/fpstovall/nrtb_beta).
 
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
 
#ifndef base_cond_var_test_h
#define base_cond_var_test_h 
 
#include "../base_thread.h"
#include "general.h"
#include "../../include/hires_timer.h"
#include <sstream>
#include <boost/concept_check.hpp>

using namespace nrtb;
using namespace std;

class seller: public runnable
{
public:
  hirez_timer elapsed;
  int hits;
  
  seller()
  {
	message = "";
	hits = 0;
  };
  
  void signal(const string & msg)
  {
	{
	  scope_lock lock(signal_lock);
	  message = msg;
	  signal_lock.signal();
	}
	{
	  scope_lock lock(ready_lock);
	  ready_lock.timed_wait(1);
	}
  };
  
  void run()
  {
	elapsed.start(0);
	bool done = false;
	while (!done)
	{
	  scope_lock lock(signal_lock);
	  while (message == "")
		signal_lock.wait();
	  cout << elapsed.interval() << " Recieved " 
		<< message << endl;
	  if (message == "done")
	  {
		done = true;
	  };
	  hits++;
	  message = "";
	  {
		scope_lock t(ready_lock);
		ready_lock.signal();
	  }
	};
  };
  
protected:
  cond_variable signal_lock;
  cond_variable ready_lock;
  string message;
};

seller store;

class cond_test: public runnable
{
public:
  int ec;
  
  cond_test()
  {
	ec = 0;
  };
  
  void run()
  {
	for (int i=0; i < 1000; i++)
	{
	  stringstream message;
	  message << "Request_" << i;
	  store.signal(message.str());
	};
	store.signal("done");
  };

  string operator() ()
  {
	stringstream msg;
	msg << "Condition Variable Test\n"
	  << "\tSent : 1001\n"
	  << "\tRecd : " << store.hits
	  << "\n\tPassed: " << ((store.hits == 1001)?"True":"False")
	  << endl;
	ec = 1001 - store.hits;
	return msg.str();
  };
  
};


#endif // base_cond_var_test_h