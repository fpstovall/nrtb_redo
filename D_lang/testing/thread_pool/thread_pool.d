/*********************
Multithreaded message passing example in D
*********************/

// import the concurrency and standard IO modules
import std.concurrency, std.stdio;

class thread_pool(wp_t)
{
  // Housekeeping variables  
  private Tid listener_tid;
  
  // Does nothing but start the listener.
  public this(auto task) {
    listener_tid = spawn(listener_thread!wp_t(task, queue_size));
  };
    
  // submit a work, packet for processing.
  public void submit(wpt wp) {
    listener_tid.send(thisTid, wp);
  };
      
}

void worker_thread(wpt)(auto task) {
  bool running = true;
  while (running) {
    receive (
      (Tid t, wpt d) { task(t,d); },
      (OwnerTerminated e) { running = false; }
    );
  };
}

void listener_thread(wpt)(auto task, auto queue_size) {
  
  Tid worker_list[];
  int next = 0;
  
  void submit(Tid t, wpt d) {
    worker_list[next].send(t,d);
    next = (++next) % worker_list.length;
  }
  
  // initial setup.
  for(auto i = 0; i< queue_size; i++) {
    worker_list.append(spawn(worker_thread!wpt(task)));
  }
    
  // service loop
  bool running = true;
  while (running) {
    receive ( 
      (Tid t, wpt d) { submit(t,d); },
      (OwnerTerminated e) { running = false; }
    );
  };
};

// main is just like main in C++
void main()
{
  writeln("D Message Driven Work Queue Example.");
};