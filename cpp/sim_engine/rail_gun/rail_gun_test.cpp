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
  bool moving {false};
  int steps {0};
  condition_variable done_moving;
  mutex move_lock;
public:
  string last_result;
  
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
    triplet c,g,m;
    int r;
    string resp, a1, a2;
    if (rg->command("railgun status", resp))
    {
      stringstream t(resp);
      t >> a1 >> a2 >> c >> g >> m >> r;
      if ((c != current) 
        or (g != goal)
        or (r != rounds))
      {
        current = c;
        goal = g;
        rounds = r;
        moving = true;
        steps++;
        last_result = "Moving";
      }
    }
    else if (moving)
    {
      stringstream output;
      output << "stop(" << steps << "):" << c << g << r;
      cout << output.str() << endl;
      last_result = output.str();
      moving = false;
      steps = 0;
      unique_lock<mutex> lock(move_lock);
      done_moving.notify_all();
    };
  };
 
  string wait_for_move()
  {
    unique_lock<mutex> lock(move_lock);
    done_moving.wait(lock);
    return last_result;
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
    cannon = std::make_shared<rail_gun_mk1>(*this);
    monitor = std::make_shared<gun_mon>(cannon,*this);
    mass = 1;
  };
  
  
  bool apply_collision(object_p o, float quanta) 
  {
    return true;
  };

  object_p clone()
  {
    auto returnme = std::make_shared<my_object>(*this);
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
  std::this_thread::sleep_for(chrono::milliseconds(1500));
    
  // Create test bot.
  auto b1 = std::make_shared<my_object>();
  sim.add_object(b1);
  
  // test goal seek
  stringstream cmd;
  string resp;
  cmd << "rail_gun aim " << triplet(0,-pi,pi/4);
  b1->cannon->command(cmd.str(), resp);
  
  std::this_thread::sleep_for(chrono::milliseconds(1000));
/*  failed = failed 
    or (b1->monitor->wait_for_move() 
      != "stop(7):(0,-3.14159,0.785398)(0,-3.14159,0.785398)100");

  cmd.str("");  
  cmd << "rail_gun aim " << triplet(0,pi,pi/4);
  b1->cannon->command(cmd.str(), resp);
  failed = failed 
    or (b1->monitor->wait_for_move() 
      != "stop(2):(0,3.14159,0.785398)(0,3.14159,0.785398)100");
*/
/*    
  b1->cannon->train(triplet(0,0,0));
  failed = failed 
    or (b1->monitor->wait_for_move() 
      != "stop(7):(0,0,0)(0,0,0)100");

  b1->cannon->train(triplet(100,0,0));
  failed = failed 
    or (b1->monitor->wait_for_move() 
      != "stop(4):(100,0,0)(100,0,0)100");
  cout << "goal seeking : " << (failed ? "FAIL" : "PASS") << endl;
  
  // test unconditional fire.
  int start = sim.obj_status().size();
  b1->cannon->fire(false);
  b1->monitor->wait_for_move();
  int end = sim.obj_status().size();
  failed = failed or (end != ++start);
  cout << "fire(false) : " << ((end == start) ? "PASS" : "FAIL") << endl;
*/    
  // test fire on stable.
  //-- point to new location
  //-- set fire on stable while we are moving.
  //-- wait on stable.
  //-- see if we fired.
    
  // kill another object
  cout << "Stopping Sim" << endl;
  sim.stop_sim();
  
    
  cout << "=========== rail_gun test " 
    << (failed ? "FAILED" : "PASSED") << " ============="
    << endl;
  
  return failed;
};



























