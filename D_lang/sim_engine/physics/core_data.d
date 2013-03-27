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

import std.string;
import nrtb.common.vect3d;

// =====  housekeeping messages ====== //
struct kicker {
  uint starttime;
  uint tick;
}

struct impact {
  object impactor;
  uint quanta;
}

struct core_status {
  object o;
  uint quanta;
}

// ===== internal data structures ===== //

struct current_status {
  unit quanta;
  uint last_quanta;
  uint ms_used;
  uint starttime;
  uint msgs_in;
  uint msgs_out;
}

alias pure real(ref object, long uint time) mod_func;

struct object {
  uint id;
  string name;
  string[string] attributes;
  vect3d postion;
  vect3d attitude;
  vect3d velocity;
  vect3d rotation;
  vect3d thrust;
  vect3d torque;
  real mass;
  real radius; // temp for initial alpha
  // message queue to send updates to.
  Tid wrapper_tid;
  // modifier functon lists
  mod_func[] modifiers;
}

struct world {
  object[uint] objects;
}