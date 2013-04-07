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
import core_data;

void run_quanta(Tid t, ref current_status c, ref world w) {
  c.last_quanta++;
  uint real_time = c.last_quanta * c.quanta; // sim time in ms.
  real interval = c.quanta * 0.001; // convert ms to seconds.

  // apply movement
  foreach(Tid t, sim_object o; w.objects) {
    // apply functional modifications
    foreach(mod_func f; w.modifiers[o.id]) {
      f(o,real_time);
    }
    // update rates
    o.velocity = o.velocity + ((o.thrust * o.mass) * interval);
    o.rotation = o.rotation + ((o.torque * o.mass) * interval); // nota good model!!
    // move it
    o.position = o.position + (o.velocity * interval);
    o.attitude = o.attitude + (o.rotation * interval);
    // send update to wrapper;
    t.send(o,c.last_quanta);
  }
 /* 
  // simple boundary sphere check for collisions
  auto tobjs = w.objects.dup;
  auto l = tobjs.length;
  for(auto i=0; i<l-1; i++) {
    auto a = tobjs[i];
    for (auto j=i+1; j<l; j++) {
      auto b = tobjs[j];
      if (a.position.range(b.position) < (a.radius + b.radius)) {
	// notify those involved in the collision.
	impact ti;
	ti.quanta = c.quanta;
	ti.impactor = a;
	b.wrapper_tid.send(ti);
	ti.impactor = b;
	a.wrapper_tid.send(ti);
      }
    }
  }
*/
  }