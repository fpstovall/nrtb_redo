/*********************
Multithreaded message passing example in D
*********************/

// import the concurrency and standard IO modules
import std.concurrency, std.stdio, core.thread, std.string;

struct thread_pool(wp_t, alias task)
{
  // Housekeeping variables  
  private Tid listener_tid;
  
  // Does nothing but start the listener.
  public this(string name, int queue_size) {
    listener_tid = spawn(&listener_thread!(wp_t,task),name,queue_size);
  };
    
  // submit a work, packet for processing.
  public void submit(wp_t wp) {
    listener_tid.send(thisTid, wp);
  };
      
}

void worker_thread(wpt, alias task)(string id) {
  writeln(id," started.");
  bool running = true;
  int c = 0;
  while (running) {
    receive (
      (Tid t, wpt d) { task(t,d); c++; },
      (OwnerTerminated e) { running = false; }
    );
  };
  writeln(id," processed ",c," messages.");
}

void listener_thread(wpt, alias task)(string name, int queue_size) {
  
  Tid worker_list[];
  ulong next = 0;
  
  void submit(Tid t, wpt d) {
    worker_list[next].send(t,d);
    next = (++next) % worker_list.length;
  }
  
  // initial setup.
  writeln(name," listener started");
  for(int i = 0; i< queue_size; i++) {
    string id = format("%s_worker_%d",name,i);
    worker_list ~= spawn(&worker_thread!(wpt, task), id);
  }
    
  // service loop
  bool running = true;
  int sent = 0;
  while (running) {
    receive ( 
      (Tid t, wpt d) { submit(t,d); sent++; },
      (OwnerTerminated e) { running = false; }
    );
  };
  writeln(name," queued ",sent," messages to workers.");
};

//==== code starts here

void mytask(Tid t, int i) {
  Thread.sleep(dur!("msecs") (i));
};


// main is just like main in C++
void main()
{
  writeln("D Message Driven Work Queue Example.");
  auto myqueue = thread_pool!(int,mytask)("thread_pool",3);
    
  for (auto i=0; i<100; i++) {
    myqueue.submit(i);
  };
  
  //Thread.sleep(dur!("seconds") (3));
  writeln("All messages sent.");
};