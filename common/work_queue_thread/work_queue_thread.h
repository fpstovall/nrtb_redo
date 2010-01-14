/* provides the basic thread for writing to a database.

	TODO: Needs to be adapted to us boost threads.

*/

#ifndef work_queue_thread_h
#define work_queue_thread_h

#include <base_thread.h>
#include <fstream>
#include <queue>
#include <iostream>

namespace ricklib
{

/** Implements a general purpose queue driven worker thread framework.
 ** 
 ** work_queue_threads are useful where you need one or more threads to
 ** process data asyncronously from the feed rate. work_queue_threads can
 ** operate singlely or in teams working the same input queue.  Example
 ** applictions could be as a connection handler on a web server, or any
 ** place where you need to business while allowing a "slow" task (i.e.
 ** remote communications) to complete without affecting the primary task.
 ** 
 ** This template takes two class arguments to instanciate. The first,
 ** workpacket, is the data type that will be passed in and queued for the
 ** threads to work on. The second, threadclass, must implement the same
 ** interface as ricklib::thread, and is in fact generally expected to be
 ** ricklib::thread or one of descendents such as ricklib::free_thread.
 ** 
 ** Things to remember about classes instanciated from work_queue_thread:
 ** 
 ** 1. All instances of a class based on work_queue_thread with the
 **    same parameters share the same work queue, and will share in
 **    processing the data placed in that queue by the store() method. As
 **    long as there are one or more instances of a given class running,
 **    the data will processed as soon as thread can get to it.
 **    
 ** 2. If you need different classes descended from work_queue_thread
 **    with the same workpacket and thread types, the optional q_uid
 **    template parameter (the third one) when instanciating the
 **    work_queue_threads. q_uid defaults to 0 if not specified, but can be
 **    specified to any valid int value, allowing you have many different
 **    queues that were instanciated with the same data types. If you don't
 **    use q_uid, all work_queue_thread descendents instanciated with the
 **    same workpacket and threadclass types _will_ share the same work
 **    queue, even if they have different do_run() methods.
 **    
 ** 3. If you need a different queue type than the default
 **    queue<workpacket> you can use the optional template parameter seq
 **    (the fourth parameter) to specify a different queue type. It will
 **    expected to provide push(), front() and pop() methods, and store and
 **    return the same datatype specified in the first template parameter.
 **    Use this if you need a priority queue for example.  Of course, if
 **    you want to specify the queue type, you'll need to specify the q_uid
 **    parameter discussed above.
 **    
 ** 4. Data passed to the class using the store() method will be queued,
 **    even if the there are no instances running at that time (both the
 **    data queue and store method are static). processing will start at
 **    the front of the queue in FIFO order once an instance is started.
 **    
 ** 5. Please note that there is no way provided in this template
 **    to get data back from the thread once it is processed. If you need
 **    feedback, you'll need to build the mechanism yourself.
 **    
 ** 6. As with all derivitives of ricklib::runnable, you can not safely
 **    put a work_queue_thread in most STL constainers because they reserve
 **    the right to move things around at whim. Instead, derive a
 **    boost::shared_ptr for your class, create your instances on the heap,
 **    and put the shared_ptrs in the container instead. Don't use auto_ptr
 **    because it's definately not safe in containers; in fact Containers
 **    Of AutoPtrs (COAPs) are explictitly disallowed by the current C++
 **    standard.
 **    
 ** Usage Notes:
 ** 
 ** You'll need to create a descendent of work_queue_thread and override
 ** do_work() at a minimum to use this. If you need to keep track of
 ** threads after they are created keep smart pointers to them in a
 ** container as noted in #6 above. Don't override run() as you would with
 ** other descendents of ricklib::runable as it controls the park,do_work
 ** cycle.  Instead put code to accomplish the needed task in do_work(),
 ** which is abstract in the template anyway.
**/
template <class workpacket, class threadclass=boost::thread, 
	int q_uid = 0, class seq = std::queue<workpacket> > 
class work_queue_thread : public threadclass
{
	public:
		/** Stores the workpacket in the pending work queue.
		 ** 
		 ** Returns immediately and the message is processed asyncronously.
		 ** Generally you will not want to override this method.
		**/
		static void store(const workpacket & message);
		/** Returns the number of threads awaiting work.
		 ** 
		 ** Another way to look at this that it represents the number of
		 ** running threads the class that are currently idle awaiting work.
		 ** When the threads are idle, they are not consuming CPU cycles.
		**/
		static unsigned int parked();
		/** Returns the number of workpackets awaiting processing.
		 ** 
		 ** Technically, this returns pending.size(), or the number of
		 ** elements in the deque that stores work packets not yet picked up
		 ** for processing by a thread. Assuming you have one or more running
		 ** instances of your class, this should be zero most of the time
		 ** unless you've managed to get all your threads busy at once.
		 ** 
		 ** If you occasionally check this value and keep finding it > 0, you
		 ** might consider creating more instances of your class.
		**/
		static unsigned int work_pending();
		/** Releases a stalled thread.
		 ** 
		 ** Technically, this method signals the condition variable to force
		 ** the thread to check the pending queue. Generally this should not
		 ** be needed. It is provided to deal with the POSIX legal situation
		 ** where a signal generated by the store() method could be ignored
		 ** by the thread. If you are truely paranoid, have some other
		 ** process in your application call this every few seconds just
		 ** ensure that no one got stuck.
		 ** 
		 ** Note that while POSIX says it is acceptable to miss the
		 ** occasional signal, I've never seen it happen on a modern system.
		 ** If you have data hanging in the queue (work_pending() > 0 and
		 ** parked() > 0) you can call this method to start processing again.
		 ** I don't expect you'll need it.
		**/
		static void kick_start();
	protected:
		/** latch protects data and available as well as signalling when
		 ** there is new data to process.  Use work_queue_thread::store() to
		 ** store the workpacket for later processing.
		**/
		static cond_variable latch;
		static seq pending;
		static int available;
		// thread local data. 
		workpacket my_data;
		/** called when the thread had data to process.
		 ** 
		 ** Override this to do the work you need to do. You'll find the data
		 ** in my_data, which will be of whatever type you specified to make
		 ** the template. This is where you'll actually get the work on the
		 ** workpacket done.
		**/
		virtual void do_work() = 0;
		/** called by the thread when it is waiting for data.
		 ** 
		 ** Avoid overriding this.
		**/
		virtual void park();
		/** the thread's main execution loop.
		 ** 
		 ** Avoid overriding this.
		**/
		virtual void run();
};

// initialize the static variables.
template <class workpacket, class threadclass, int q_uid, class seq>
	cond_variable work_queue_thread<workpacket,threadclass,q_uid,seq>::latch;
template <class workpacket, class threadclass, int q_uid, class seq>
	seq work_queue_thread<workpacket,threadclass,q_uid,seq>::pending;
template <class workpacket, class threadclass, int q_uid, class seq>
	int work_queue_thread<workpacket,threadclass,q_uid,seq>::available = 0;

template <class workpacket, class threadclass, int q_uid, class seq>
	void work_queue_thread<workpacket,threadclass,q_uid,seq>
		::store(const workpacket & message)
{
	scope_lock lock(latch);
	pending.push(message);
	latch.signal();
};

template <class workpacket, class threadclass, int q_uid, class seq>
	unsigned int work_queue_thread<workpacket,threadclass,q_uid,seq>
		::parked()
{
	scope_lock lock(latch);
	return available;
};

template <class workpacket, class threadclass, int q_uid, class seq>
	unsigned int work_queue_thread<workpacket,threadclass,q_uid,seq>
		::work_pending()
{
	scope_lock lock(latch);
	return pending.size();
};

template <class workpacket, class threadclass, int q_uid, class seq>
	void work_queue_thread<workpacket,threadclass,q_uid,seq>
		::kick_start()
{
	scope_lock lock(latch);
	latch.signal();
};

template <class workpacket, class threadclass, int q_uid, class seq>
	void work_queue_thread<workpacket,threadclass,q_uid,seq>
		::park()
{
	scope_lock lock(latch);
	pthread_cleanup_push(ricklib::safe_unlock,&latch);
	// increment the number of threads available.
	available++;
	// wait until there is data to work with.
	// NOTE: this latch.wait() is a thread cancelation point.
	while (pending.empty()) { latch.wait(); };
	// take the message as our own and clear the buffer.
	my_data = pending.front();
	pending.pop();
	// decrement the number of threads available.
	available--;
	pthread_cleanup_pop(0);
};

template <class workpacket, class threadclass, int q_uid, class seq>
	void work_queue_thread<workpacket,threadclass,q_uid,seq>
		::run()
{
	while (true)
	{
		try
		{
			park(); // this is a cancellation point
			do_work();
		}
		catch (std::exception & e)
		{
			std::clog << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__
				<< ": WARNING!! unexpected exception caught." << std::endl
				<< "\t" << e.what() << std::endl;
			throw e;
		};
	};
};

}; // namespace NRTB

#endif // work_queue_thread_h
