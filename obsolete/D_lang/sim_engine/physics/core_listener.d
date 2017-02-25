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

import std.concurrency, std.stdio, core.thread, std.string;
import core_listener, core_messages, run_quanta;

public static struct sim_core_listener {

  Tid listener_tid;
  
  // does nothing but start the listenr
  public this() {
    listener_tid = spawn(&listener_thread)
  }
    
}

private void listener_thread() {

  current_status c;
  world w;

  // ==== service loop ===== //
  bool running = true;
  while (running) {
    receive (
      (Tid t, kicker) { run_quanta(t,c,w); },
      (OwnerTerminated e) { running = false; }
    );
  };
}

private void kicker_thread(Tid listener, uint quantms) {
  bool running = true;
  uint quanta = 0;
  
  while (running) {
    listener.send(kicker(0,quanta);
    quanta++;
    Thread.sleep(dur!("msecs")(quantams));
  }
}
