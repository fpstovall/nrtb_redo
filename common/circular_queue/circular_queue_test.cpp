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
 
#include <string>
#include <iostream>
#include "circular_queue.h"
#include <boost/shared_ptr.hpp>

using namespace nrtb;
using namespace std;

typedef circular_queue<int> test_queue;
typedef boost::shared_ptr<test_queue> queue_p;

class consumer_task: public thread
{
public:

  consumer_task(string n, queue_p buffer)
  {
	name = n;
	input = buffer;
	count = 0;
  };
  
  ~consumer_task()
  {
	cout << ">> in " << name << "::~consumer_task()" << endl;
	try
	{
	  this->thread::~thread();
	  input.reset();
	}
	catch (...)  {};
	cout << "<< leaving " << name << "::~consumer_task()" << endl;
  };
  
  int get_count() { return count; };
  
  void run()
  {
	try
	{
	  while (true)
	  {
		int num = input->pop();
		{
		  static mutex console;
		  scope_lock lock(console);
		  cout << name << " picked up " << num
			<< endl;
		};
		count++;
		lastnum = num;
		yield();	  
	  }
	}
	catch (...) {};
  };

protected:
  // link to the feed queue
  queue_p input;
  // a name to report 
  string name;
  // number of items processed
  int count;
  // last number caught
  int lastnum;
};

typedef boost::shared_ptr<consumer_task> task_p;


int main()
{
  int er_count = 0;
  /************************************************
   * Load queue and then cook it down...
   ***********************************************/
  // make and load a queue
  queue_p q1(new test_queue(50));
  for (int i=0; i<100; i++)
  {
	q1->push(i);
  };
  // the queue should be loaded with 50-99
  // attach a thread and process it.
  task_p p1(new consumer_task("task 1",q1));
  p1->start();
  while (q1->size()) usleep(100);
  cout << "cp 1 " << p1->get_count() << endl;
  /************************************************
   * now that the preload is exhasted, shove items
   * in one at a time to make sure each is picked
   * up correctly.
   ***********************************************/
  for (int i=200; i<225; i++)
  {
	q1->push(i);
	usleep(100);
  };
  cout << "cp 2 " << p1->get_count() << endl;
  /************************************************
   * Last check; attach a second thread to the queue
   * and make sure both are servicing it.
   ***********************************************/
  task_p p2(new consumer_task("task 2",q1));
  p2->start();
  for (int i=300; i<325; i++)
  {
	q1->push(i);
  };
  while (q1->size()) usleep(100);
  // shut it all down
  q1->shutdown();
  p1->join();
  p2->join();
  // important numbers
  int tot_items = p1->get_count() + p2->get_count();
  int p1_items = p1->get_count() - 75;
  int p2_items = p2->get_count();
  // release she threads and queues.
  p1.reset();
  p2.reset();
  q1.reset();
  // do some reporting.
  cout << "cp 3 " 
	<<  tot_items
	<< " [75 + (" << p1_items
	<< " + " << p2_items 
	<< ")]" << endl;
	bool passed = (tot_items == 100);
  // inverted logic needed because 0 is good for 
  // return codes.
  return !passed; 
};