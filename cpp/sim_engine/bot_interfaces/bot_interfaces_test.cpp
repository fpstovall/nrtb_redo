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

#include "bot_interfaces.h"
#include <hires_timer.h>
#include <base_object.h>
#include <iostream>
#include <string>
#include <condition_variable>
#include <future>

using namespace nrtb;
using namespace std;

template <class T>
class test_module
{
public:
  test_module(T & p) : parent(p) {};
  void test_send_to_bcp()
  {
    parent.send_to_bcp("BCP message");
  }
  void test_bot_cmd()
  {
    parent.bot_cmd("Bot command");
  }
  void test_wait_for_tick()
  {
    hirez_timer clock;
    clock.start();
    parent.wait_for_tick();
    cout << "Slept for " << clock.stop() << " seconds." << endl;
  }
private:
  T & parent;
};

struct my_object
: public abs_bot
{
  test_module<my_object> module;
  
  my_object() 
    : module(*this)
  {
    location = triplet(0,0,0);
    velocity = triplet(0,0,0);
    bounding_sphere.center = triplet(0,0,0);
    bounding_sphere.radius = 1;
    mass = 1;
  };

  my_object(my_object & o)
    : module(*this)
  {
    location = o.location;
    velocity = o.velocity;
    bounding_sphere = o.bounding_sphere;
    attitude.set(o.attitude);
    rotation.set(o.rotation);
  };
  
  virtual void send_to_bcp(string msg)
  {
    cout << "send_to_bcp(): " << msg << endl;
  };  
  
  virtual void bot_cmd(string cmd)
  {
    cout << "bot_cmd(): " << cmd << endl;
  };
  
  virtual void wait_for_tick()
  {
    // this tickable implementation simply plugs
    // into the object's tick method.
    unique_lock<mutex> lock(tick_lock);
    ticker.wait(lock);
  };
  
  bool apply_collision(object_p o) 
  {
    return true;
  };

  bool tick(int quanta)
  {
    // release threads waiting on tick.
    ticker.notify_all();
    // call original tick method.
    return base_object::tick(quanta);
  };
  
  base_object * clone()
  {
    my_object * returnme = new my_object(*this);
    returnme->pre_attribs = get_pre_attribs_copy();
    returnme->post_attribs = get_post_attribs_copy();
    return returnme;
  };
private:
  // variables for ticker implementation.
  condition_variable ticker;
  mutex tick_lock;
};

void tick_test(int loop, my_object &o)
{
  for(int i=0; i<loop; i++)
    o.module.test_wait_for_tick();
};

int main()
{
  bool failed = false;
  cout << "========== bot_interfaces test ============="
    << endl;

  my_object o1;

  o1.module.test_send_to_bcp();  

  o1.module.test_bot_cmd();
  
  int iterations(5);
  chrono::milliseconds pause(20);
  auto task = async(launch::async,tick_test,iterations,std::ref(o1));
  for(int i=0; i<iterations; i++)
  {
    this_thread::sleep_for(pause);
    o1.tick(i);
  };
    
  task.wait();
  
  if (failed)
    cout << " *** Unit test failed" << endl;
    
  cout << "=========== bot_interfaces test complete ============="
    << endl;
  
  return failed;
};



























