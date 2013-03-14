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

pure void run_quanta(Tid, t, ref current_status c, ref world w) {
  c.last_quanta++;
  t = c.last_quanta * 20;

  // apply movement
  // -- apply functional changes
  foreach(object o; w.objects) {
    foreach(mod_func f; o.modifiers) {
      f(o,t);
    }
  }
  
  // simple boundary sphere check for collisions.
  
  // Send updates the to object wrappers as appropriate.

}