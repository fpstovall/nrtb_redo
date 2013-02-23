/*********************
Multithreaded message passing example in D
*********************/

// import the concurrency and standard IO modules
import std.concurrency, std.stdio, core.thread;

struct thread_pool(wp_t, alias task)
{
  // Housekeeping variables  
  private Tid listener_tid;
  
  // Does nothing but start the listener.
  public this(int queue_size) {
    listener_tid = spawn(&listener_thread!(wp_t,task),queue_size);
  };
    
  // submit a work, packet for processing.
  public void submit(wp_t wp) {
    listener_tid.send(thisTid, wp);
  };
      
}

void worker_thread(wpt, alias task)() {
  bool running = true;
  while (running) {
    receive (
      (Tid t, wpt d) { task(t,d); },
      (OwnerTerminated e) { running = false; }
    );
  };
}

void listener_thread(wpt, alias task)(int queue_size) {
  
  Tid worker_list[];
  ulong next = 0;
  
  void submit(Tid t, wpt d) {
    worker_list[next].send(t,d);
    next = (++next) % worker_list.length;
  }
  
  // initial setup.
  for(int i = 0; i< queue_size; i++) {
    worker_list ~= spawn(&worker_thread!(wpt,task));
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

//==== code starts here

void mytask(Tid t, int i) {
  writeln("Task ",thisTid," processed ",i);
  Thread.sleep(dur!("msecs") (20));
};


// main is just like main in C++
void main()
{
  writeln("D Message Driven Work Queue Example.");
  auto myqueue = thread_pool!(int,mytask)(10);
  
  for (auto i=0; i<100; i++) {
    myqueue.submit(i);
  };
  
  Thread.sleep(dur!("seconds") (3));
  writeln("Run complete");
  
};