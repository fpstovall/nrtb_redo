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

pure void run_quanta(Tid t, ref current_status c, ref world w) {
  c.last_quanta++;
  real_time = c.last_quanta * c.quanta; // sim time in ms.
  interval = c.quanta * 0.001; // convert ms to seconds.

  // apply movement
  foreach(object o; w.objects) {
    // apply functional modifications
    foreach(mod_func f; o.modifiers) {
      f(o,real_time);
    }
    // update rates
    o.velocity += (o.thrust * o.mass) * interval;
    o.rotation += (o.torque * o.mass) * interval; // nota good model!!
    // move it
    o.position += o.velocity * interval;
    o.attitude += o.rotation * interval;
    // send update to wrapper;
    o.wrapper.send(o);
    
  }
  
  // simple boundary sphere check for collisions
  l = w.objects.length;
  for(auto i=0; i<l-1; i++) {
    a = w.objects[i];
    for (auto j=i+1; j<l; j++) {
      b = w.objects[j];
      if (a.position.magnitude(b.position) < (a.radius + b.radius)) {
	// notify those involved in the collision.
	impact i;
	i.quanta = c.quanta;
	i.impactor = a;
	b.wrapper.send(i);
	i.impactor = b;
	a.wrapper.send(i);
      }
    }
  }
}