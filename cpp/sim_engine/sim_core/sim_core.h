/***********************************************
 This file is part of the NRTB project (https://github.com/fpstovall/nrtb_beta).
 
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
 * Truthful information is provided on all
 * objects; it's up to caller to limit or 
 * confuse things. :)
 *****************************************/
class panopticon
{
public:
  // Returns all objects in the system.
  contacts get();
  /** the following should only be
   ** used only by sim_core; */
  // - flags the start of a list load operation.
  void start_new();
  // - Add an object to the list.
  void add(const base_object & o);
  // -- marks the list load operation complete.
  void done_adding();
private:
  contacts c_list;
  contacts t_list;
  std::mutex list_lock;
};
  
class sim_core
{
public:
  // constructor, default quanta is 50 hz.
  sim_core(float time_slice=1.0/50.0);
  /***************************************
   * control methods.
   **************************************/
  // Override the quanta set at construction
  // Note: throws if the similation is running.
  void set_quanta(float time_slice);
  // returns true if running.
  bool running();
  // returns the current population count
  int size();
  // Starts the simulation. Throws if already running.
  void start_sim(int threads);
  // Stops the the simulation. 
  void stop_sim();
  // Inserts the object into the simulation.
  void add_object(object_p obj);
  // Removes the object from the simulation.
  void remove_obj(unsigned long long oid);
  // returns the contact list (panopticon::get()).
  contacts contact_list();
  /**************************************
   * Reporting methods.
   *************************************/
  // reporting record to be stored on "sim_output" ipc_queue
  struct report
  {
    unsigned long long quanta;
    unsigned long long duration;
    double wallclock;
    strlist objects;
  };
  /****************************************
   * The following methods are exposed for 
   * reporting and testing and should not be
   * called directly in "real" code.
   ***************************************/
  // returns the as_str() for each object in the simulation
  strlist obj_status();
  // Returns clones of each opbject in the simulation.
  object_list get_obj_copies();
private:
  /****************************************
  * clsn_rec is used to track intersecting
  * objects.
  ***************************************/
  struct clsn_rec
  {
    object_p a;
    object_p b;
  }; 
  // message queue and adaptors.
  ipc_queue messages;
  gp_sim_message_adapter q;
  // general purpose internal messaging interface.
  gp_sim_message_p next_out_message();
  void put_message(nrtb::gp_sim_message_p & m);
  // General run data (self-descriptive.
  std::atomic<bool> end_run;
  std::atomic<bool> is_running;
  std::atomic<unsigned long long> quanta;
  float quanta_duration;
  // Thread handle for simulation engine.
  std::thread engine;
  // Lists (self descriptive.
  object_list all_objects;
  std::vector<clsn_rec> collisions;
  std::vector<unsigned long long> deletions;
  panopticon public_list;
  // these methods implement simulation steps.
  void turn_init();
  void tick();
  static clsn_rec check_one(object_p a, object_p b, float duration);
  void collision_check();
  void resolve_collisions();
  // Stores the current state to the "sim_output" ipc_queue
  //  in a gp_sim_message with report struct as the data.
  report get_report(unsigned long long ticks, double wt);
  // The actual simulation engine.. runs as a seperate thread.
  void run_sim();
  /***************************
   * Changes for parallel update processing.
   ****************************/
  int worker_count {2};
  typedef linear_queue<object_p> object_q;
  object_q ticklist;
  object_q applylist;
  void do_tick();
  void do_apply();  
};

typedef singleton<sim_core> global_sim_core;
  
} // namepace nrtb

#endif // sim_core_header
