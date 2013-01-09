/*********************
Multithreaded message passing example in D
*********************/

// import the concurrency and standard IO modules
import std.concurrency, std.stdio;

class thread_pool(wp_t)
{
  // struct used to send messages in.
  public struct in_msg{Tid sender, wp_t data};
  // Housekeeping variables
  private uint _low_water = 2;
  private uint _high_water = 0;
  private uint _increment = 2;
  private Tid listener_tid;
  private Tid[] all_workers
  private Tid[] available_workers
  
  // Does nothing but start the listener and _low_water workers
  private this()
  {
    listener_tid = spawn(&this.listener_thread)
    for(auto i=0; i<_low_water; i++)
    {
      mk_new_worker();
    };
  };
  
  abstract void do_work(inout in_msg packet);

  final:
    private void mk_available(in Tid tid)
    {
      available_workers.append(tid);
    };
    
    private Tid get_next_worker()
    {
      Tid returnme = available_workers.front()
      available_workers.pop_front();
      return returnme;
    };
    
    private void assign_work(in in_msg packet)
    {
      // do we have enough workers?
      auto c = available_workers.length();
      if (c < _low_water)
      {
	if ((_high_water==0) or (all_workers.length() < _high_water))
	{
	  // create new workers as needed
	  for(auto i=0; i<_increment; i++)
	  {
	    mk_new_worker();
	  };
      };
      // message the first worker
      Tid myworker = get_next_worker();
      myworker.send(thisTid, packet);
    };
    
    private void mk_new_worker()
    {
      Tid worker = spawn(&worker_thread);
      available_workers.push_back(worker);
      all_workers.push_back(worker);
    };
    
    private void listener_thread()
    {
      // simple response loop
      running = true;
      while running
      {
	recieve(
	  (in_msg p) { assign_work(p); }
	  (OwnerTerminated e) { running = false; }
	);
      };
    };
    
    private void worker_thread()
    {
      // simple response loop
      running = true;
      while running
      {
	recieve(
	  (in_msg p) { do_work(p); }
	  (OwnerTerminated e) { running = false; }
	);
      };
    };
  
};


// main is just like main in C++
void main()
{
  writeln("D Message Driven Work Queue Example.");

  // intialize some variable; the compiler can figure out the type from usage.
  auto low=0, high = 100;

  // launch a second thread using function receiver defined below.
  auto rtid = spawn(&receiver);

  // foreach iterates a range; in this case all values between
  // low and high are assigned are assigned to i sequencially.
  foreach (i; low .. high)
  {
    // sends a message to receiver using it's thread id; queue management is automatic.
    rtid.send(i);
    // report our action
    writeln("Sent ", i);

  }
  writeln("** main() is complete **");
}

// receiver is a function which is run as a seperate thread by main.
void receiver()
{
  writeln("** receiver() started. **");
  
  // a variable for error checking.
  auto lastid = -1;

  // function to handle messages
  void int_handler(int i)
  {
    // just print a line and record the last id received
    writeln(" Received ",i);
    lastid = i;
  }

  // Loop until we the parent shuts down.
  bool running = true;
  while (running)
  {
    receive
    (
      // handler for int messages
      (int msg) { int_handler(msg); },
      // handler for "owner terminated" exception
      (OwnerTerminated e) { running = false; }
    );
  }

  // quick check to be sure we got them all.
  assert(lastid == 99);
  writeln("** receiver() is complete. **");
}