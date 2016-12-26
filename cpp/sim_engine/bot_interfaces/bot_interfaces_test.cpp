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

class test_module
{
public:
  test_module(abs_bot & p) : my_parent(p) {  };

  void test_send_to_bcp()
  {
    my_parent.send_to_bcp("BCP message");
  };

  void test_bot_cmd()
  {
    my_parent.bot_cmd("Bot command");
  };

private:
  abs_bot & my_parent;
  hirez_timer clock;
};

struct my_object
: public abs_bot
{
  test_module module;
  
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
    
  bool apply_collision(object_p o, float duration) 
  {
    return true;
  };
    
  object_p clone()
  {
    my_object * returnme = new my_object(*this);
    returnme->pre_attribs = get_pre_attribs_copy();
    returnme->post_attribs = get_post_attribs_copy();
    return object_p(returnme);
  };
  
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
  for(int i=0; i<iterations; i++)
  {
    this_thread::sleep_for(pause);
    o1.tick(1.0/50.0);
  };
      
  if (failed)
    cout << " *** Unit test failed" << endl;
    
  cout << "=========== bot_interfaces test complete ============="
    << endl;
  
  return failed;
};



























