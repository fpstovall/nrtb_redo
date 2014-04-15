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
 
#ifndef sim_core_header
#define sim_core_heade

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
  
struct clsn_rec
{
  unsigned long long a;
  unsigned long long b;
};
  
class sim_core
{
public:
  sim_core(unsigned long long quanta_microsec);
  /***************************************
   * control methods.
   **************************************/
  bool running();
  void start_sim();
  void stop_sim();
  void add_object(object_p obj);
  object_p remove_obj(unsigned long long oid);
  gp_sim_message_p next_out_message();
  void put_message(gp_sim_message_p m);
  /**************************************
   * Reporting methods.
   *************************************/
  strlist obj_status();
  object_list get_obj_copies();
  
private:
  ipc_queue messages;
  std::atomic<bool> is_running;
  std::atomic<unsigned long long> quanta;
  object_list all_objects;
  std::vector<clsn_rec> collisions;
  std::vector<unsigned long long> deletions;
  object_p deep_copy(object_p);
  void turn_init(quanta);
  void tick(unsigned long long quanta);
  void collision_check();
  static void run_sim(sim_core & world);
};

typedef singleton<sim_core> global_sim_core;
  
} // namepace nrtb

#endif // sim_core_heade
