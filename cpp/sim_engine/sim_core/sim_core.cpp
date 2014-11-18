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

// see sim_core.h for documentation

#include "sim_core.h"
#include <unistd.h>
#include <hires_timer.h>
#include <ipc_channel.h>
#include <common_log.h>
#include <sstream>

using namespace std;
using namespace nrtb;

contacts panopticon::get()
{
  std::unique_lock<std::mutex>(list_lock);
  return c_list;
};

void panopticon::start_new()
{
  t_list.clear();
};

void panopticon::add(const base_object & o)
{
  sensor_rec s;
  s.type = 1;
  s.id = o.id;
  s.location = o.location;
  s.velocity = o.velocity;
  s.radius = o.bounding_sphere.radius;
  t_list.push_back(s);
};

void panopticon::done_adding()
{
  std::unique_lock<std::mutex>(list_lock);
  c_list = t_list;
};

sim_core::sim_core(float time_slice)
  : quanta(0), 
    quanta_duration(time_slice),
    end_run(false),
    is_running(false),
    q(gp_sim_message_adapter(messages))
{};

void sim_core::set_quanta(float time_slice)
{
  if (!is_running and quanta==0)
  {
    quanta_duration = time_slice;
  }
  else
  {
    base_exception e;
    e.store("Invalid call to sim_core::set_quanta()");
    throw e; 
  };
};

sim_core::report sim_core::get_report(unsigned long long ticks, double wt)
{
  report returnme;
  returnme.quanta = quanta;
  returnme.duration = ticks;
  returnme.wallclock = wt;
  returnme.objects = get_obj_copies();
  return returnme;
};

strlist sim_core::obj_status()
{
  strlist returnme;
  for(auto o: all_objects)
  {
    returnme.push_back(o.second->as_str());
  };
  return returnme;
};

object_list sim_core::get_obj_copies()
{
  object_list returnme;
  for(auto o: all_objects)
  {
     object_p t(o.second->clone());
     returnme[o.first] = t;
  };
  return returnme;
};

contacts sim_core::contact_list()
{
  return public_list.get();
};


void sim_core::tick()
{
  // call the local tick and apply for each object in the simulation.
  for(auto & a: all_objects)
  {
    // have to use this method to make sure the apply step is not
    // optimized out by the compiler in some conditions.
    bool killme = a.second->tick(quanta);
    bool killme2 = a.second->apply(quanta, quanta_duration);
    // mark the object for deletion if appropriate.
    if (killme or killme2)
      deletions.push_back(a.first);
  };
  // at the end of this method, all objects are either
  // at their final state ignorning collisions or deleted.
};

void sim_core::collision_check()
{
  // this version is using a very naive algorithm.
  auto b = all_objects.begin();
  auto c = b;
  auto e = all_objects.end();
  while (c != e)
  {
    b = c;
    b++;
    while (b != e)
    {
      if (c->second->check_collision(b->second))
      {
        // a collision has been found.
        clsn_rec crec;
        crec.a = c->second;
        crec.b = b->second;
        collisions.push_back(crec);
      };
      b++;
    };
    c++;
  };
  // at exit, all colliions have been recorded.
};


void sim_core::turn_init()
{
  // Process all the messages in queue at this point.
  for(int i=messages.size();i>0;i--)
  {
    gp_sim_message_p msg = q.pop();
    // check for proper message type
    if (msg->msg_type() != 0)
    {
      // We'll treat this as a hard error.
      stringstream s;
      s << "bad msg in sim_core::turn_init(): "
	      << msg->as_str();
      base_exception e;
      e.store(s.str());
      throw e;
    };
    const int noun_obj(1);
    const int noun_ctl(2);
    switch (msg->noun())
    {
      case noun_obj:
      {
      	const int verb_add(1);
      	const int verb_rm(2);
      	switch (msg->verb())
      	{
      	  case verb_add:
      	  {
      	    // Add new object to list.
      	    // TODO: We may want to check for unique id.
      	    auto new_obj= msg->data<object_p>();
      	    all_objects[new_obj->id] = new_obj;
      	    break;
      	  }
      	  case verb_rm:
      	  {
      	    // maark object for deletion.
      	    auto did=msg->data<unsigned long long>();
      	    deletions.push_back(did);
      	    break;
      	  }
      	  default:
      	  {
      	    // unhandled message
      	    stringstream s;
      	    s << "Unhanded object verb in sim_core::turn_init: "
      	      << msg->as_str();
      	    base_exception e;
      	    e.store(s.str());
      	    throw e;
      	    break;
      	  };
      	};
      	break;
      }
      case noun_ctl:
      {
      	const int verb_stop(1);
      	switch (msg->verb())
      	{
      	  case verb_stop:
      	  {
            end_run = true;
      	    break;
      	  }
      	  default:
      	  {
      	    // unhandled message
      	    stringstream s;
      	    s << "Unhanded control verb in sim_core::turn_init: "
      	      << msg->as_str();
      	    base_exception e;
      	    e.store(s.str());
      	    throw e;	
      	    break;
      	  }
      	}
      }
      default:
      {
      	// unhandled noun; treat as a hard error.
      	stringstream s;
      	s << "bad noun in sim_core::turn_init(): "
      	  << msg->as_str();
      	base_exception e;
      	e.store(s.str());
      	throw e;
      	break;
      }
    };
  };
  /* conduct all cleanup needed to ensure the sim
     is a proper state for the next turn. */
  // Clear out the collision list
  collisions.clear();
  // delete any objects marked in the last turn.
  for (auto a : deletions)
  {
    // ignore errors here.
    try { all_objects.erase(a); } catch (...) {};
  };
  // clear the deletions list
  deletions.clear();
};

void sim_core::resolve_collisions()
{
  for (auto crec : collisions)
  {
    // call each object's collision routine
    bool killa = crec.a->apply_collision(crec.b);
    bool killb = crec.b->apply_collision(crec.a);
    // move out of conflict if alive and intersecting.
    if (!(killa or killb))
    {
      base_object & a = *(crec.a);
      base_object & b = *(crec.b);
      double min_dist = a.bounding_sphere.radius 
        + b.bounding_sphere.radius; 
      double adj = min_dist - a.location.range(b.location);
      // is a move still needed?
      if (adj > 0.0)
      {  
         // TODO: this too naive to be allowed to live
         // overlong. Needs to be replaced by a version
         // which produces more realistic offsets..
         adj /= 2;
         triplet vec = (a.location-b.location).normalize();
         a.location += vec * adj;
         b.location -= vec * adj;
      };
    };
    // kill or adjust objects as needed.
    if (killa) { remove_obj(crec.a->id); };
    if (killb) { remove_obj(crec.b->id); };
  };
};

void sim_core::put_message(gp_sim_message_p & m)
{
  q.push(m);
};

gp_sim_message_p sim_core::next_out_message()
{
  return q.pop();
};

void sim_core::add_object(object_p obj)
{
  if (is_running)
  {
    // assemble the message.
    void_p d(new object_p(obj));
    gp_sim_message_p g(new gp_sim_message(messages,0,1,1,d));
    // queue the message.
    put_message(g);
  }
  else 
  {
    all_objects[obj->id] = obj;
  };
};

void sim_core::remove_obj(long long unsigned int oid)
{
  if (is_running)
  {
    // assemble the message
    void_p d(new unsigned long long(oid));
    gp_sim_message_p g(new gp_sim_message(messages,0,1,2,d));
    // queue the message.
    put_message(g);
  }
  else
  {
    all_objects.erase(oid);
  };
};

void sim_core::stop_sim()
{
  end_run = true;
  if (engine.joinable())
    engine.join();
};

void sim_core::start_sim()
{
  // are we already running?
  if (is_running)
  {
    // Report the error.
    base_exception e;
    e.store("Already running in sim_core::start_sim().");
    throw e; 
  };
  // launch the run_sim() method.
  engine =  std::thread(sim_core::run_sim,std::ref(*this));
};

bool sim_core::running()
{
  return is_running;
};


void sim_core::run_sim(sim_core & w)
{
  w.is_running = true;
  // link to sim engine general log
  log_recorder glog(common_log::get_reference()("sim_core::run"));
  glog.trace("Starting");
  try
  {
    // establish output links.
    // -- sim_core output channel
    ipc_channel_manager& ipc
      = global_ipc_channel_manager::get_reference();
    ipc_queue & oq = ipc.get("sim_output");
    gp_sim_message_adapter output(oq);
    // output initial state
    glog.trace("Storing inital model state");
    void_p r(new report(w.get_report(0,0.0)));
    // -- for init, type 1, noun 0, verb 0 carries a report struct.
    output.push(new gp_sim_message(oq, 1, 0, 0, r));
    glog.trace("Entering game cycle");
    // start wall-clock timer.
    hirez_timer wallclock; // governs the overall turn time
    hirez_timer turnclock; // measures the actual gonculation time.
    w.quanta=0;
    unsigned long long ticks = round(w.quanta_duration * 1e6);
    unsigned long long nexttime = ticks;
    while (!w.end_run)
    {
      turnclock.reset();
      turnclock.start();
      w.quanta++;
      w.turn_init();
      w.tick();
      w.collision_check();
      w.resolve_collisions();
      // populate public sensor list;
      w.public_list.start_new();
      for(auto i: w.all_objects)
      {
        w.public_list.add(*i.second);
      };
      w.public_list.done_adding();
      // output turn status
      unsigned long long elapsed = turnclock.interval_as_usec();
      void_p r(new report(w.get_report(elapsed,wallclock.interval())));
      // -- for output, type 1, noun 1, verb 0 carries a report struct.
      output.push(new gp_sim_message(oq, 1, 1, 0, r));
      // check for overrun and report as needed.
      if (elapsed >= ticks)
      {
        base_exception e;
        stringstream s;
        s << "Quanta " << w.quanta << " Overrun: " 
          << elapsed << " usec expected "
          << ticks;
        e.store(s.str());
        throw e;
      }; 
      // sleep until the next turn by the wall clock
      unsigned long long tosleep = nexttime - wallclock.interval_as_usec();
      usleep(tosleep);
      nexttime += ticks;
    };
  }
  // Catch most errors in our code..
  catch (base_exception &e)
  {
    glog.critical(e.what());
    glog.critical(e.comment());
    glog.critical("Run termimnated abnormally.");
  }
  // catch most errors in library code...
  catch (std::exception &e)
  {
    glog.critical(e.what());
    glog.critical("Run terminated abnormally.");
  }
  // unconditionally catch any other errors.
  catch (...)
  {
    glog.critical("Undefined exception");
    glog.critical("Run terminated abnormally.");
  };
  // close out nicely.
  glog.trace("complete");
  w.is_running = false;
};


