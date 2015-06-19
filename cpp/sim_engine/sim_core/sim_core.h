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
 
#ifndef sim_core_header
#define sim_core_header

#include <vector>
#include <map>
#include <memory>
#include <thread>
#include <triad.h>
#include <serializer.h>
#include <singleton.h>
#include <base_object.h>
#include <messages.h>

namespace nrtb
{

/**************************************
 * sensor_rec is the individual contact 
 * in the list returned to callers 
 * by panopticon::get().  Coordinates
 * and velocity are world-relative.
 *************************************/  
struct sensor_rec
{
  // type of object
  int type;
  // Object id
  unsigned long long id;
  // where it is
  triplet location;
  // how fast it's going
  triplet velocity;
  // how large is it.
  float radius; 
};

/***************************************
 * These define how the list of contacts
 * is passed from panopticon::get() to
 * the caller.
 **************************************/
typedef std::vector<sensor_rec> contacts;
typedef std::shared_ptr<contacts> contacts_p;

/******************************************
 * panopticon provides the functionality 
 * used by the simulation engine to provide
 * contact information to external modules.
 *****************************************/
class panopticon
{
public:
  // public accessor
  contacts get();
  // used only by sim_core;
  void start_new();
  void add(const base_object & o);
  void done_adding();
private:
  contacts c_list;
  contacts t_list;
  std::mutex list_lock;
};
  
struct clsn_rec
{
  object_p a;
  object_p b;
};

class sim_core
{
public:
  sim_core(float time_slice=1.0/50.0);
  /***************************************
   * control methods.
   **************************************/
  void set_quanta(float time_slice);
  bool running();
  void start_sim();
  void stop_sim();
  void add_object(object_p obj);
  void remove_obj(unsigned long long oid);
  // general purpose work thread interface.
  gp_sim_message_p next_out_message();
  void put_message(nrtb::gp_sim_message_p& m);
  /**************************************
   * Reporting methods.
   *************************************/
  // -- reporting record
  struct report
  {
    unsigned long long quanta;
    unsigned long long duration;
    double wallclock;
    object_list objects;
  };
  strlist obj_status();
  object_list get_obj_copies();
  contacts contact_list();
  
private:
  ipc_queue messages;
  gp_sim_message_adapter q;
  std::atomic<bool> end_run;
  std::atomic<bool> is_running;
  std::atomic<unsigned long long> quanta;
  std::thread engine;
  float quanta_duration;
  object_list all_objects;
  std::vector<clsn_rec> collisions;
  std::vector<unsigned long long> deletions;
  panopticon public_list;
  void turn_init();
  void tick();
  void collision_check();
  void resolve_collisions();
  report get_report(unsigned long long ticks, double wt);
  static void run_sim(sim_core & w);
};

typedef singleton<sim_core> global_sim_core;
  
} // namepace nrtb

#endif // sim_core_heade
