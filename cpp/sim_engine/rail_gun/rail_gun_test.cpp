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

#include <sim_core.h>
#include "rail_gun.h"
#include <base_object.h>
#include <bot_interfaces.h>
#include <iostream>
#include <string>

using namespace nrtb;
using namespace std;

typedef shared_ptr<rail_gun_mk1> rail_p;

class gun_mon : public tickable
{
private:
  rail_p rg;
  triplet current;
  triplet goal;
  abs_bot & parent;
  int rounds {0};

public:
  gun_mon(rail_p gun, abs_bot & p) 
    : rg(gun), parent(p) 
  {
    parent.register_ticker(*this);
  };
  
  virtual ~gun_mon() 
  {
    parent.deregister_ticker(*this);
  };
  
  void operator ()(float duration)
  {
    triplet c; 
    triplet g;
    int r;
    rg->get_status(c,g,r);
    if ((c != current) 
      or (g != goal)
      or (r != rounds))
    {
      cout << "rg : " << c << g << r << endl;
      current = c;
      goal = g;
      rounds = r;
    };
  };
  
};

struct my_object : public abs_bot
{
  rail_p cannon;
  shared_ptr<gun_mon> monitor;
  
  
  my_object()
  {
    location = triplet(0,0,0);
    velocity = triplet(0,0,0);
    bounding_sphere.center = triplet(0,0,0);
    bounding_sphere.radius = 1;
    cannon.reset(new rail_gun_mk1(*this));
    monitor.reset(new gun_mon(cannon,*this));
    mass = 1;
  };
  
  
  bool apply_collision(object_p o, float quanta) 
  {
    return true;
  };

  base_object * clone()
  {
    my_object * returnme = new my_object(*this);
    returnme->pre_attribs = get_pre_attribs_copy();
    returnme->post_attribs = get_post_attribs_copy();
    return returnme;
  };
  
  void bot_cmd(string cmd) 
  {
    cout << "cmd: " << cmd << endl;
  };
  
  void send_to_bcp(string msg) 
  {
    cout << "stb: " << msg << endl;
  };
};


int main()
{
  bool failed = false;
  cout << "========== rail_gun test ============="
    << endl;
 
  // create and start sim engine
  sim_core & sim = global_sim_core::get_reference();
  sim.start_sim();
    
  // Create test bot.
  my_object * b1 = new my_object;
  sim.add_object(object_p(b1));
  
  // test goal seek
  b1->cannon->train(triplet(0,-pi,pi/4));
  this_thread::sleep_for(chrono::milliseconds(500));
  cout << "---------------------" << endl;
  b1->cannon->train(triplet(0,pi,pi/4));
    
  
  // test unconditional fire.
    
  // test fire on stable.
    
  // kill another object
    

  this_thread::sleep_for(chrono::milliseconds(500));
  
  sim.stop_sim();
    
  cout << "=========== rail_gun test complete ============="
    << endl;
  
  return failed;
};



























