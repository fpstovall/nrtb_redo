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

#include "sim_core.h"
#include "messages.h"
#include "common_log.h"
#include "base_object.h"
#include <iostream>
#include <string>
#include <unistd.h>

using namespace nrtb;
using namespace std;

struct gravity : public abs_effector
{
  triplet g = triplet(0,-9.81,0);
  
  gravity()
  {
    handle = "gravity";
  };
  
  abs_effector * clone()
  {
    return new gravity(*this);
  };
  
  std::string as_str()
  {
    std::stringstream returnme;
    returnme << handle << "_" << id << "=" << g;
    return returnme.str();
  };
  
  bool tick(base_object & o, int time)
  {
    o.accel_mod += g;
    return false;
  };
};

struct impactor : public abs_effector
{
  abs_effector * clone()
  {
    return new impactor(*this);
  };
  
  std::string as_str()
  {
    std::stringstream returnme;
    returnme << "floor_impactor_" << id;
    return returnme.str();
  };
  
  bool tick(base_object & o, int time)
  {
    if ((o.location.y <= 0.0) 
      and (o.velocity.y < 0.0))
    {
      return true;
    }
    else
    {
      return false;
    };
  };  
  
};

struct rocket : public abs_effector
{
  triplet impulse = triplet(0.0,10000.0,0.0);
  float burn_time = 3.0;
  bool active = false;
  
  rocket(float duration)
  {
    burn_time = duration;
  };
  
  abs_effector * clone()
  {
    return new rocket(*this);
  };
  
  std::string as_str()
  {
    std::stringstream returnme;
    returnme << "rocket_" << id << "=" << impulse;
    return returnme.str();
  };
  
  bool tick(base_object & o, int time)
  {
    if ((burn_time > 0.0) and active)
    {
      // assumes a 50hz cycle.
      o.force += impulse;
      burn_time -= (1/50.0);
    }
    else
    {
      impulse = 0;
    };
    return false;
  };  
};

struct my_object : public base_object
{
  bool apply_collision(object_p o) 
  {
    return false;
  };
  
  base_object * clone()
  {
    my_object * returnme = new my_object(*this);
    returnme->pre_attribs = get_pre_attribs_copy();
    returnme->post_attribs = get_post_attribs_copy();
    return returnme;
  };
};

struct rocket_ball : public my_object
{
  rocket_ball()
  {
    mass = 100;
    bounding_sphere.center = triplet(0);
    bounding_sphere.radius = 0.5;
    // add gravity effects
    add_pre(new gravity);
    // add an active rocket with a 3 second impulse
    rocket * r = new rocket(3);
    r->active = true;
    add_pre(r);
    // die when we hit the floor.
    add_post(new impactor);
  };
};

bool log_test(log_recorder & l, string s, bool failed)
{
  if (failed)
  {
    cout << " ** " << s << " FAILED" << endl;
    l.critical(s+" FAILED");
  }
  else
  {
    l.info(s+" passed");
  };
  return failed;
};

int main()
{
  bool failed = false;
  log_recorder log(common_log::get_reference()("sim_core_test"));
  log.trace("Starting");
  cout << "=========== sim core test ============="
    << endl;

  sim_core world(1/50.0); // set a 50 hz run cycle.
  // test static insert of two objects
  world.add_object(object_p(new rocket_ball));
  world.add_object(object_p(new my_object));
  bool t = log_test(log,"Static object add",
                (world.obj_status().size() != 2));
  failed = failed or t;
  // test static removal of one object
  world.remove_obj(1);
  t = log_test(log,"Static object remove",
                (world.obj_status().size() != 1));
  failed = failed or t; 
  
  for (auto s : world.obj_status())
    cout << s << endl;
  
  // engine start test.
  auto engine = world.start_sim();
  t = log_test(log,"sim_core::start_sim() returned",false);
  failed = failed or t;
  // run 3 and running test.
  sleep(3);
  t = log_test(log,"sim_run() running",!world.running());
  failed = failed or t; 
  // stop test.
  world.stop_sim();
  t = log_test(log,"sim_core::stop_sim() returned",false);
  failed = failed or t; 
  engine.join();
  t = log_test(log,"sim_run() stopped",world.running());
  failed = failed or t; 

  
  for (auto s : world.obj_status())
    cout << s << endl;
  
  log_test(log,"Unit Test",failed);
  
  cout << "=========== sim_core test complete ============="
    << endl;
  log.trace("Run Complete");
  return failed;
};



























