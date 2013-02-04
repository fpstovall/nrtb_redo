/*********************
Multithreaded message passing example in D
*********************/

// import the concurrency and standard IO modules
import std.concurrency, std.stdio;

class thread_pool(wp_t)
{
  public:
    alias wp_t my_t;
  
    // submit a work packet for processing.
    final void submit(in my_t wp) {
      listener_tid.send(thisTid, wp);
    };
    
    // get/set queue parameters
    final uint set_ctl_params(ref uint l, ref uint h, ref uint i) {
      set_values packet;
      packet.low = l;
      packet.high = h;
      packet.inc = i;
      packet.current = 0;
      listener_tid.send(thisTid, packet);
      receive(
	(set_values s) { packet = s; }
      );
      l = packet.low;
      h = packet.high;
      i = packet.inc;
      return packet.current;
    }
    
    // 
  
  // do_work is responsible for returning any responses to 
  // the requesting thread if needed.
  abstract void do_work(in Tid requestor, in my_t data);
  
  // Does nothing but start the listener.
  private this()
  {
    listener_tid = spawn(&this.listener_thread, thisTid);
  }
    
  // struct used to send/receive messages in.
  private struct set_values{uint low; uint high; uint inc; uint current;};
  private struct shutdown{bool immediate;};
  // Housekeeping variables
  private Tid listener_tid;
  

  final:
    
    private void listener_thread()
    {
      struct thread_data{Tid thread; uint start_time; uint hits;};
      alias thread_data[Tid] tlist;
      tlist active_threads;
      tlist available;
      uint low_water = 2; 
      uint high_water = 0;
      uint increment = 2;
      
      void mk_thread() {
	thread_data t;
	t.thread = spawn(&worker_thread);
	t.data = 0;
	t.hits = 0;
	active_threads[t.thread] = t;
	available[t.thread] = t;
      }
      
      void assign_work(in Tid req, in my_t wp) {
	// check to see if we have enough workers
	if ((available.length < low_water) 
	  && (high_water != 0) 
	  && (available < high_water)
	) {
	  for (auto i=0; i<increment; i++) 
	    mk_thread();
	}  
	// assign the job
	Tid target = available.keys[0];
	available.remove(target);
	target.send(thisTid, req, wp);
	active_threads[target].hits++;
      }
      
      void task_done(Tid t) {
	available[t] = active_threads[t];
      }
      
      void handle_params(in Tid caller, in set_values params) {
	if (l > 1) { low_water = params.low; };
	if (h > -1) { high_water = params.high; };
	if (i > 0) { increment = params.inc; };
	params.low = low_water;
	param.high = high_water;
	params.inc = increment;
	params.current = active_threads.length;
	t.send(params);
      }
      
      // simple response loop
      bool running = true;
      while (running)
      {
	recieve(
	  (in_msg p)		{ assign_work(p); },
	  (Tid t)    		{ task_done(t); },
	  (Tid t, set_values s)	{ handle_params(t,s); },
	  (OwnerTerminated e) 	{ running = false; },
	);
      };
    };
    
    private void worker_thread() {
      // simple response loop
      bool running = true;
      while (running)
      {
	recieve(
	  (Tid t, tid r, my_t p) { do_work(r,p); t.send(thisTid);  },
	  (OwnerTerminated e) { running = false; }
	);
      };
    };
  
};

class mypool: thread_pool!int {

  override void do_work(in Tid requestor, in my_t data) {
    double printme = 0.0;
    for(auto i=0; i<data; i++) {
      for (auto a=i; a<a+100; a++) {
	printme += a * a;
      }
    }
    writeln("worker ",thisTid," returns ",printme);
  }

}

// main is just like main in C++
void main()
{
  writeln("D Message Driven Work Queue Example.");
};