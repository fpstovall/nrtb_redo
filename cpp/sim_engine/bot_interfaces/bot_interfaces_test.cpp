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
#include <base_object.h>
#include <iostream>
#include <string>
#include <condition_variable>

using namespace nrtb;
using namespace std;

struct my_object
: public base_object, 
  public bcp_sender,
  public cmd_interface,
  public tickable
{
  my_object() 
  {
    location = triplet(0,0,0);
    velocity = triplet(0,0,0);
    bounding_sphere.center = triplet(0,0,0);
    bounding_sphere.radius = 1;
    mass = 1;
  };

  my_object(my_object & o)
  {
    location = o.location;
    velocity = o.velocity;
    bounding_sphere = o.bounding_sphere;
    attitude.set(o.attitude);
    rotation.set(o.rotation);
  };
  
  virtual void send_to_bcp(string msg)
  {
    cout << "TO BCP: " << msg << endl;
  };  
  
  virtual string bot_cmd(string cmd)
  {
    cout << "CMD: " << cmd << endl;
  };
  
  virtual void wait_for_tick()
  {
    unique_lock<mutex> lock(tick_lock);
    ticker.wait(lock);
  };
  
  bool apply_collision(object_p o) 
  {
    return true;
  };

  bool tick(int quanta)
  {
    ticker.notify_all();
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
  condition_variable ticker;
  mutex tick_lock;
};

int main()
{
  bool failed = false;
  cout << "========== bot_interfaces test ============="
    << endl;
 
  
  if (failed)
    cout << " *** Unit test failed" << endl;
    
  cout << "=========== bot_interfaces test complete ============="
    << endl;
  
  return failed;
};



























