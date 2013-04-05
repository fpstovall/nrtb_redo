/***********************************************
This file is part of the NRTB project (https://launchpad.net/nrtb).

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

import std.string, std.concurrency, std.conv;
import nrtb.common.vect3d;

// =====  housekeeping messages ====== //

struct kicker {
  uint starttime;
  uint tick;
}

struct start_sim {
  string sim_name;
  uint quanta_ms;
  bool allow_late_entry;
  bool randomize_start_positions;
}

struct stop_sim {
  string sim_name;
  bool force_bot_disconnect;
}

struct sim_starting {
  uint quantams;
  sim_object you_are_here;
  bool clock_running;
}

struct sim_ended {
  uint quanta;
  sim_object final_status;
  bool prep_for_restart;
}

// Object maint messages

struct impact {
  sim_object impactor;
  uint quanta;
}

struct obj_status {
  sim_object o;
  uint quanta;
}

struct add_obj {
  sim_object new_obj;
  bool allow_random_placement;
}

struct removed_obj {
  sim_object final_status;
  string reason;
  bool may_restart;
  bool errored;
}

struct apply_force {
  uint id;
  vect3d translation;
  vect3d rotation;
}

struct set_attribute {
  uint id;
  string name;
  string value;
}

struct unset_attribute {
  uint id;
  string name;
}

struct contact_list {
  uint quanta;
  sim_object[uint] contacts;
}

// ===== internal data structures ===== //

struct current_status {
  uint quanta;
  uint last_quanta;
  uint ms_used;
  uint starttime;
  uint msgs_in;
  uint msgs_out;
}

alias pure void function(ref sim_object, uint time) mod_func;

struct sim_object {
  uint id;
  string name;
  vect3d position;
  vect3d attitude;
  vect3d velocity;
  vect3d rotation;
  vect3d thrust;
  vect3d torque;
  double mass;
  double radius; // temp for initial alpha
}

alias mod_func[uint] mod_func_list;

struct world {
  sim_object[Tid] objects;
  mod_func_list[Tid] modifiers;
  string[Tid][string] attributes;
}