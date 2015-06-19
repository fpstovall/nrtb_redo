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
  
  effector_p clone()
  {
    return effector_p(new gravity(*this));
  };
  
  std::string as_str()
  {
    std::stringstream returnme;
    returnme << handle << "_" << id << "=" << g;
    return returnme.str();
  };
  
  bool tick(base_object & o, float quanta)
  {
    o.accel_mod += g;
    return false;
  };
};

struct impactor : public abs_effector
{
  bool peak = false;

  effector_p clone()
  {
    return effector_p(new impactor(*this));
  };
  
  std::string as_str()
  {
    std::stringstream returnme;
    returnme << "floor_impactor_" << id;
    return returnme.str();
  };
  
  bool tick(base_object & o, float quanta)
  {
    if ((o.location.y <= 0.0) 
      and (o.velocity.y < 0.0))
    {
      // impact!!
      return true;
    }
    else
    {
      // apogee test..
      if ((o.location.y > 0.1) and (o.velocity.y <= 0.0) and (!peak))
      {
        peak = true;
      };
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
  
  effector_p clone()
  {
    return effector_p(new rocket(*this));
  };
  
  std::string as_str()
  {
    std::stringstream returnme;
    returnme << "rocket_" << id << "=" << impulse;
    return returnme.str();
  };
  
  bool tick(base_object & o, float quanta)
  {
    if ((burn_time > 0.0) and active)
    {
      // assumes a 50hz cycle.
      o.force += impulse;
      burn_time -= quanta;
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

struct rocket_ball : public my_object
{
  rocket_ball()
  {
    mass = 100;
    bounding_sphere.center = triplet(0);
    bounding_sphere.radius = 0.5;
    // add gravity effects
    add_pre(effector_p(new gravity));
    // add an active rocket with a 3 second impulse
    rocket * r = new rocket(0.13);
    r->active = true;
    add_pre(effector_p(r));
    // die when we hit the floor.
    add_post(effector_p(new impactor));
  };
};

bool log_test(log_recorder & l, string s, bool failed)
{
  if (failed)
  {
    cout << " ** " << s << ": FAILED" << endl;
    l.critical(s+" FAILED");
  }
  else
  {
    l.info(s+": passed");
  };
  return failed;
};

typedef map<string,float> val_map;
val_map get_sim_metrics()
{
  // set up access.
  auto & ipc = global_ipc_channel_manager::get_reference();
  ipc_queue & soq = ipc.get("sim_output");
  gp_sim_message_adapter sim_out(soq);
  // total it up.
  int c = soq.size();
  // gather data
  float avg_ticks = 0.0;
  float max_ticks = 0.0;
  float min_ticks = 1e6;
  float avg_wall = 0.0;
  float max_wall = 0.0;
  float min_wall = 1e6;
  float wb = 0.0;
  int counted = 0;
  while (soq.size())
  {
    gp_sim_message_p msg = sim_out.pop();
    sim_core::report qr = msg->data<sim_core::report>(); 
    // discard quanta < 2; interval lacks a full turn.
    if (qr.quanta > 1)
    {
      counted++;
      avg_ticks += qr.duration;
      max_ticks = qr.duration > max_ticks ? qr.duration : max_ticks;
      min_ticks = qr.duration < min_ticks ? qr.duration : min_ticks;
      float wt = qr.wallclock - wb;
      avg_wall += wt;
      max_wall = wt > max_wall ? wt : max_wall;
      min_wall = wt < min_wall ? wt : min_wall;
      wb = qr.wallclock;
    };
  };
  avg_ticks /= counted;
  avg_wall /= counted;
  // store it all
  val_map returnme;
  returnme["turns"] = c;
  returnme["counted"] = counted;
  returnme["avg_usec"] = avg_ticks;
  returnme["max_usec"] = max_ticks;
  returnme["min_usec"] = min_ticks;
  returnme["avg_wall"] = avg_wall;
  returnme["max_wall"] = max_wall;
  returnme["min_wall"] = min_wall;
  returnme["run_time"] = wb;
  return returnme;
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
  
  cout << "Single object run (3 secs)" << endl;
  world.start_sim();
  t = log_test(log,"sim_core::start_sim() returned",false);
  failed = failed or t;
  // run 3 and running test.
  sleep(3);
  t = log_test(log,"sim_run() running",!world.running());
  failed = failed or t; 
  // stop test.
  world.stop_sim();
  t = log_test(log,"sim_run() stopped",world.running());
  failed = failed or t; 

  // object remove test (one died during the above run)
  t = log_test(log,"Dynamic object remove",world.obj_status().size());
  failed = failed or t; 

  // get the results for the first run.
  val_map metrics = get_sim_metrics();
  log.info("First run data");
  for (auto a : metrics)
  {
    stringstream s;
    s << a.first << "=" << a.second;
    log.info(s.str());
  };
  // verify the run results.
  int c = floor(metrics["turns"]);
  t = log_test(log,"Output record count", ((c < 150) or (c>155)));
  failed = failed or t;
  t = log_test(log,"Quanta RT limit", (metrics["max_wall"]>0.03));
  failed = failed or t;
  t = log_test(log,"Quanta RT pace", (metrics["avg_wall"]>0.02001));
  failed = failed or t;
  t = log_test(log,"Quanta usec limit", (metrics["max_usec"]>1.5e4));
  failed = failed or t;

  cout << "Dynamic add test (0.3 secs)" << endl;
  sim_core w1(1/50.0);
  w1.start_sim();
  while (!(w1.running())) usleep(10);
  usleep(1e5);
  contacts l1 = w1.contact_list();
  w1.add_object(object_p(new rocket_ball));
  usleep(1e5);
  contacts l2 = w1.contact_list();
  usleep(1e5);
  w1.stop_sim();
  object_list objs = w1.get_obj_copies();
  t = log_test(log,"Dynamic object add", (objs.size() != 1));
  failed = failed or t;
  t = (l1.size() != 0) or (l2.size() != 1);
  t = log_test(log,"contact_list()",t);
  failed = failed or t;
  // output l2
  for (auto i : l2)
    cout << i.id << ":" << i.type << ":"
      << i.location << ":" << i.velocity
      << ":" << i.radius << endl;
  // check metrics again
  metrics = get_sim_metrics();
  log.info("Second run data");
  for (auto a : metrics)
  {
    stringstream s;
    s << a.first << "=" << a.second;
    log.info(s.str());
  };
  // verify the run results.
  t = log_test(log,"Quanta RT limit", (metrics["max_wall"]>0.03));
  failed = failed or t;
  t = log_test(log,"Quanta RT pace", (metrics["avg_wall"]>0.0201));
  failed = failed or t;
  t = log_test(log,"Quanta usec limit", (metrics["max_usec"]>1.5e4));
  failed = failed or t;
  
  cout << "50 objects/50 hz (3 secs)" << endl;
  sim_core w2(1/50.0);
  for(int i=0; i<50; i++)
  {
    object_p r(new rocket_ball);
    r->location.x = i * 5.0;
    w2.add_object(r);
  };
  // verify starting count
  t = log_test(log,"50/50 start count", (w2.obj_status().size()!=50));
  failed = failed or t;
  w2.start_sim();
  sleep(3);
  w2.stop_sim();
  // verify ending count (all should have been distroyed.
  t = log_test(log,"50/50 end count", (w2.obj_status().size()));
  failed = failed or t;
  
  // Verify metrics one last time.
  metrics = get_sim_metrics();
  log.info("50/50 run data");
  for (auto a : metrics)
  {
    stringstream s;
    s << a.first << "=" << a.second;
    log.info(s.str());
  };
  // verify the run results.
  c = floor(metrics["turns"]);
  t = log_test(log,"Output record count", ((c < 150) or (c>155)));
  failed = failed or t;
  t = log_test(log,"Quanta RT limit", (metrics["max_wall"]>0.03));
  failed = failed or t;
  t = log_test(log,"Quanta RT pace", (metrics["avg_wall"]>0.02001));
  failed = failed or t;
  t = log_test(log,"Quanta usec limit", (metrics["max_usec"]>1.5e4));
  failed = failed or t;

  cout << "Collision test (1 secs)" << endl;
  sim_core w3(1/50.0);
  w3.add_object(object_p(new rocket_ball));
  object_p target(new my_object);
  target->location.y = 0.75;
  w3.add_object(target);
  w3.start_sim();
  sleep(1);
  w3.stop_sim();
  while (w3.running()) usleep(50);
  for (auto o : w3.get_obj_copies())
    cout << o.second->as_str() << endl;
  // verify ending count (all should have been distroyed.
  t = log_test(log,"Collision test", (w3.obj_status().size()));
  failed = failed or t;
    
  log_test(log,"Unit Test",failed);
  
  cout << "=========== sim_core test complete ============="
    << endl;
  log.trace("Run Complete");
  return failed;
};



























