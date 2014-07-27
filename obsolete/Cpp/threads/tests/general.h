/***********************************************
 This file is part of the NRTB project (https://github.com/fpstovall/nrtb_alpha).
 
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
 
#ifndef base_general_test_h
#define base_general_test_h 
 
#include "../base_thread.h"
#include "../../include/hires_timer.h"
#include <sstream>
#include <boost/smart_ptr.hpp>

using namespace nrtb;
using namespace std;

class console_writer
{
public:
  mutex mylock;
  hirez_timer elapsed;
  
  void write(const string & output)
  {
	scope_lock lock(mylock);
	cout << elapsed.interval() << ": " << output << endl;
  };
};

console_writer console;

class gen_test_task: public runnable
{
  public:

	int loop_counter;
	int count_to;
	string name;
	int desired_priority;
	hirez_timer runtime; 
	
	gen_test_task(
	  const string & text,
	  int target,
	  int priority
	)
	{
	  loop_counter = 0;
	  count_to = target;
	  name = text;
	};
	
    virtual ~gen_test_task() {};
	
	void run()
	{
	  runtime.start(0);
	  set_cancel_anytime();
	  stringstream msg;
	  msg.clear();
	  msg << name
		<< "(" << count_to << "," << desired_priority 
		<< ") started.";
	  console.write(msg.str());
	  // loop till we die.
	  for (int i=0; i<count_to; i++)
	  {
		loop_counter++;
		// yeild the timeslice
		sleep(0);
	  };
	  // report status and stop runtime counter.
	  msg.str("");
	  msg << name << " has counted to " << loop_counter;
	  console.write(msg.str());
	  runtime.stop();
	};
	
	string report()
	{
	  stringstream maker;
	  maker << "Task " << name << " Report"
		<< "\n\tIterations  : " << loop_counter
		<< "\n\tRun Time    : " << runtime.interval()
		<< endl;
	  return maker.str();
	};
};

class gen_tester
{
  public:
	typedef boost::shared_ptr<gen_test_task> taskp;
	typedef vector<taskp> task_list;
	typedef vector<thread> thread_list;
	task_list tasks;
	thread_list threads;
	hirez_timer runtime;
	int ec;
	
	gen_tester()
	{
	  ec = 0;
	  for (int i=0; i<10; i++)
	  {
		stringstream s;
		s << "task_" << i;
		taskp t(new gen_test_task(s.str(),1e5,i*10));
		tasks.push_back(t);
		thread p(*t);
		threads.push_back(p);
	  };
	};
	
	void run()
	{
	  stringstream msg;
	  msg.str("");
	  msg << "Starting " << threads.size() << " threads.";
	  console.write(msg.str());
	  // start all threads
	  thread_list::iterator s = threads.begin();
	  thread_list::iterator e = threads.end();
	  thread_list::iterator c = s;
	  while (c != e)
	  {
		c->start();
		c++;
	  };
	  msg.str("");
	  msg << "All threads started.";
	  console.write(msg.str());
	  // wait for the threads to complete.
	  c = s;
	  while (c != e)
	  {
		c->join();
		c++;
	  };
	  msg.str("");
	  msg << threads.size() << " threads complete.";
	  console.write(msg.str());
	  runtime.stop();
	};
	
	string operator() ()
	{
	  stringstream msg;
	  task_list::iterator s = tasks.begin();
	  task_list::iterator e = tasks.end();
	  task_list::iterator c = s;
	  while (c != e)
	  {
		taskp t = *c;
		msg << t->report();
		if (t->loop_counter != t->count_to)
		{
		  ec++;
		  msg << "The counter does not appear correct." << endl;
		};
		c++;
	  };
	  thread_list::iterator ts = threads.begin();
	  thread_list::iterator te = threads.end();
	  thread_list::iterator tc = ts;
	  while (tc != te)
	  {
		if (tc->is_running())
		{
		  msg << "ERROR: A thread appears to be running." << endl;
		  ec++;
		};
		tc++;
	  };
	  msg << "\nGeneral test run time " << runtime.interval() 
		<< " seconds." << endl;
	  return msg.str();
	};
};

#endif // base_general_test_h