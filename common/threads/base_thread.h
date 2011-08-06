/***********************************************
 This file is part of the NRTB project (https://*launchpad.net/nrtb).
 
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
 
#ifndef base_thread_h
#define base_thread_h

#include <common.h>
//#define linux_pthreads
#include <pthread.h>

namespace nrtb 
{

/** Abstract base class for independent runnable processes.  
 **/
class runnable
{
	protected:
	
		/** Ends the thread's execution.
		 ** 
		 ** This is a polite run termination, which must be requested by the 
		 ** running thread itself instead of being imposed by by another process.
		 ** Making this method protected insures that the only code that can call it
		 ** is the code specifed in the run method. This prevents the inadvertant
		 ** shutdown of other threads.
		 **/
		void exit(int value);

		/** Prevents runtime cancellation.
		 ** 
		 ** After calling this method the thread not not be cancelled by
		 ** other threads. Use this with care; if set the only way to kill
		 ** the thread will be to kill the entire application.
		 **/
		void set_not_cancelable();

		/** Enables deferred cancellation.
		 ** 
		 ** After calling this method the a cancellation request will be deferred
		 ** until one of the pthreads defined cancellation points or until the 
		 ** thread calls test_cancel().
		 ** 
		 ** This is the default thread cancellability state. 
		 **/
		void set_deferred_cancel();

		/** Allows the thread to be cancelled at any time.
		 ** 
		 ** After calling this method the thread will be cancelled immediately
		 ** if another thread attempts to cancel it.
		 **/
		void set_cancel_anytime();

		/** Terminates the thread if another thread has requested cancellation.
		 ** 
		 ** The code in run() can call this to accept a cancel request if the 
		 ** thread is currently in deferred_cancel status. Calling this method 
		 ** if cancellation has not been requested has no effect on the threads 
		 ** execution.
		 ** 
		 ** This should be called periodically in cancelable long run threads to 
		 ** insure that cancel requests are accepted in a timely manner.
		 **/
		void test_cancel();
		
	public:
	
		/** (Abstract) The code to be run.
		 ** 
		 ** Override this class to provide the code that will be run 
		 ** in a descendent class.
		 **/
		virtual void run() = 0;

		/** Destructor.
		 ** 
		 ** Base destructor is a nop.
		 **/
		virtual ~runnable() { };
};

/** Basic thread class.
 ** 
 ** Usage: You must override the run() method or pass in a descendent of
 ** "runnable" to provide the code the thread will execute when started. Once 
 ** this is done, call start() to start the thread, and call stop to forcefully
 ** end thread execution.
 ** 
 ** NOTE: Passing out of scope will kill the thread. This is a safety thing
 ** because the private data the thread may be using becomes undefined in that
 ** case.
 ** 
 **If you are overriding the run() method to provide the thread code, you are 
 ** free create new constructor(s) to allow easy initalization of any class 
 ** data elements you've added.
 ** 
 ** If you will be passing in a runnable via the thread(runnable &) constructor
 ** see the notes with that constructor. In short, remember the actual execution
 ** thread in that case will occur in the context of the runnable descendent and 
 ** not that of the thread class, so local data must be stored in that context.
 ** 
 ** NOTE: this class is pthreads specific. Many of the public methods should 
 ** not be overridden because they provide basic thread control functionality,
 ** _unless_ you are adapting this class to us a different thread lib.
 **/
class thread: public runnable
{
	private:
	
		runnable * alt_run;
		bool start_detached;
		
	protected:
	
		pthread_t mytid;
		
		/** Provides a startpoint for threads.
		 ** 
		 ** Do NOT override this method. It exists to provide a constant
		 ** start point for pthread_create() function and contains the logic to 
		 ** determine which run method (local or one from a passed in 
		 ** runnable) we are to call.
		 **/
		static void * startpoint(void * _this);
		/// Yields processing time to another process.
		void yield();

	public:

		// Exceptions.
		/// Use this to catch all possible thread exceptions.
		class general_exception: public base_exception {};
		/// Thrown by run() if no thread code is available to run.
		class no_code_exception: public general_exception {};
		/// Thrown by start() if the thread could not be started.
		class no_start_exception: public general_exception {};
		/** Thrown if a thread attribute change is requested while the
		 ** thread is executing and changes are locked out.
		 **/
		class attr_change_failed_exception: public general_exception {};
		/// Thown if an attempt is made to join a detached thread.
		class not_joinable_exception: public general_exception {};
		/// Thrown if the requested thread could not be found.
		class thread_id_not_found_exception: public general_exception {};
		/// Thrown if an attempt to start an already running thread is made.
		class already_running_exception: public general_exception {};
		/// Thrown if the thread is not running when needed.
		class not_running_exception: public general_exception {};

		/** Default constructor.
		 ** 
		 ** This is nearly a nop in the base class, but this is okay as long as the 
		 ** run() is overridden and you don't need to initialize any local
		 ** data.
		 ** 
		 ** The only action taken by the default constructor is to to set the 
		 ** private variable alt_run = 0 to prevent accidental calling of the 
		 ** wrong run method.
		 **/
		thread();

		/** Constructs from a runnable.
		 ** 
		 ** Constructs the thread to use the code and data from the runnable
		 ** descendent class provided. If you want to provide "local" data for
		 ** the thread to use during execution it should be provided in the 
		 ** runnable descendent instead the base_thread descendent, since the 
		 ** thread will execute within the scoping context of the runnable 
		 ** descendent. (whew!!)
		 ** 
		 ** Do not override this method because it does some special handling
		 ** to insure the runnable's run() method is executed instead of the 
		 ** base_thread's.
		 **/
		thread(runnable & dothis);

		/** Kills and destructs the thread.
		 ** 
		 ** This will kill a running thread automagically. This is a safety
		 ** behavior to prevent the thread from attempting access to object
		 ** data that becomes undefined if the object moves out of scope.
		 **/
		virtual ~thread();

		/** Optionally provides the code executed by the thread.
		 ** 
		 ** Override this class to provide the actual code for the thread 
		 ** unless you are going to be passing in a descendent of runnable
		 ** to the constructor. If you do not override this and you don't 
		 ** instanciate via the base_thread(runnable) constructor, any call to 
		 ** start() is going to throw an exception.
		 **/
		virtual void run();
		
		/** Start the thread's execution.
		 ** 
		 ** If you have not overridden run() or constructed the object by 
		 ** passing in a runnable class, this method will throw a 
		 ** thread_no_code_exception. If the thread can not start for some
		 ** other reason a thread_no_start_exception is thrown. Any other 
		 ** exceptions that may be thrown by a successfully started thread
		 ** are not caught here and must be dealt with in the application code.
		 ** 
		 ** Don't override this method unless you are adapting base_thread to 
		 ** use a thread library other than pthreads.
		 **/
		void start();
		
		/** Start a thread using a supplied runnable object
		 ** 
		 ** Simply registers the runnable with the thread and
		 ** then calls start(). It is critical that the runnable
		 ** not be altered by any other process while the thread is 
		 ** running.
		 ** 
		 ** NOTE: The registered runnable object will remain 
		 ** linked to the thread object until another is passed
		 ** in.
		 **/
		void start(runnable & do_this);

		/** Rudely stop the thread's execution.
		 ** 
		 ** This actually translates to call the pthead_cancel if the thread is running,
		 ** or a nop if no thread is running. Since cancel is a rude process by 
		 ** definition, be aware that you may need to do some cleanup after calling 
		 ** this from outside the run() method.
		 ** 
		 ** Take care to prevent possible deadlocks by writing your thread code
		 ** to prevent cancellation while the thread is holding mutexes that others
		 ** may need access to later. You can do this using the set_not_cancelable(),
		 ** set_deferred_cancel(), and set_cancel_anytime() methods inherited from 
		 ** runnable.
		 ** 
		 ** Don't override this method unless you are adapting base_thread to use 
		 ** a thread library other than pthreads.
		 **/
		void stop();

		/** True if the thread is running, false otherwise.
		 **/
		bool is_running();

		/** Blocks until the thread completes.
		 ** 
		 ** If the thread is running, this will block the calling process until
		 ** the thread (i.e. the run() method) is complete. If the thread is not 
		 ** currently running this method will return immediately.
		 ** 
		 ** If the thread is detached (see detach(bool) below) a 
		 ** not_joinable_exception will the thrown.
		 **/
		int join();

		/** Makes this a "daemon" thread.
		 ** 
		 ** If the thread is detached no other thread can use join() to wait
		 ** for it's completion. However, the threads library will be able to
		 ** release some assets it must maintain on a joinable process,
		 ** saving memory.
		 ** 
		 ** If this is called during thread run: If yes==true the thread is
		 ** immediately detached if it is not already and a "note" is
		 ** recorded to set detached state immediately if the thread is
		 ** restarted later. If yes == false and the thread is already
		 ** detached an attr_change_failed_exception will be thrown because
		 ** you can not re-attach a detached thread.
		 ** 
		 ** If the thread is not currently running the detached attribute is
		 ** set for use when the thread is started.
		 ** 
		 ** Generally, if you are creating a long run thread or one that does
		 ** not terminate during the application's run (an input processor,
		 ** for example) you'll save memory if you detach it.
		 **/
		void detach(bool yes=true);
		/// returns the max priority the thread can use.
		static int max_priority(int policy = SCHED_OTHER);
		/// returns the min priority the thread can use.
		static int min_priority(int policy = SCHED_OTHER);
		/// return the thread's current priority.
		int get_priority();
		/** Sets the thread's priority
		 ** 
		 ** May throw an attr_change_failed_exception if the priority is not
		 ** acceptable (for example, the process must be running as root to
		 ** set a priority higher (numericly less) than 0.
		 ** 
		 ** The available priority range varies with operating system,
		 ** thread library, program permissions and scheduling policy in
		 ** use.  Because of these variables, it's best to use
		 ** min_priority() and max_priority() to find out the acceptable
		 ** range for sure.
		 ** 
		 ** For the record, all threads I've seen start with a priority of
		 ** 0 (normal).
		 **/
		void set_priority(const int pri, int policy = SCHED_OTHER);
};

/** Simple mutex.
 ** 
 ** Provides a mutex (MUTual EXclusion) lock. Only one thread can hold the lock
 ** at a time.
 ** 
 ** These are useful for protecting data and other resources that must be shared
 ** between two or more threads. Using mutexes is outside the scope of this 
 ** documention; a nice place to start is Chapter 3 of "Pthreads Programming" 
 ** by Nichols, Buttlar & Farrell, published by O'Reilly.
 **/
class mutex
{
	protected:
		pthread_mutex_t mymid;
	public:
		/// Use to catch all mutex exceptions.
		class general_exception: public base_exception {};
		/// Thrown when relocking a mutex already locked by the calling thread.
		class already_locked_exception: public general_exception {};
		/// Thrown when unlocking a mutex not locked by the calling thread.
		class not_owner_exception: public general_exception {};
		/// Thrown if the destructor is called on a locked mutex.
		class can_not_destruct_exception: public general_exception {};
		
		/** Constructor.
		 **/
		mutex();

		/** Destructor.
		 ** 
		 ** Care should be taken to insure that a mutex can not go out of scope
		 ** or otherwise be destructed while a thread may still expect it to be 
		 ** present.
		 **/
		virtual ~mutex();

		/** Locks the mutex.
		 ** 
		 ** If the mutex is not currently held by another thread, this locks it and
		 ** returns immediately to the caller. If the mutex is already locked by
		 ** another thread, this call will block until the mutex is unlocked 
		 ** ("becomes available") at which time it will be locked and control
		 ** returned to the caller.
		 ** 
		 ** If you really _must_ have mutexs that you can lock willy-nilly take a 
		 ** look at the recursive_mutex class. recursive_mutex can be locked
		 ** multple times by the same thread, but require the same number of unlocks
		 ** as were previously locked to release the mutex for another thread to use. 
		 ** 
		 ** NOTE: There is no time limit on how long this method will block while
		 ** attempting to gain a lock on the mutex. Therefore, once a thread locks 
		 ** a mutex, it _MUST_ unlock it when the thread is done with the resource
		 ** the mutex is protecting. Failure to do so will almost certainly result
		 ** in program deadlocks.
		 **/
		virtual void lock();

		/** Unlocks the mutex.
		 ** 
		 ** NOTE: In some pthreads implementations it is possible for a thread
		 ** to unlock a mutex owned by a different thread. The dangers in such an
		 ** action should be self-evident.  Don't do it!
		 **/
		virtual void unlock();

		/** Checks the mutex and if available locks it.
		 ** 
		 ** If the mutex is locked this method returns to the caller immediately 
		 ** with a return of "false". If the mutex is not locked, this locks it and 
		 ** returns "true" to the caller. In no case will this method block.
		 **/
		virtual bool try_lock();
};

/** Used to signal state changes between threads.
 ** 
 ** Provides a condition variable and it's associated mutex. Condition variables 
 ** are used to signal a thread that something it's interested in has changed. 
 ** Details on the use of condition variables are out of the scope of this 
 ** documention; for a good discusion check out chapeter 3 of 
 ** the book "Pthreads Programming" by Nichols, Buttlar, and Farrell, 
 ** published by O'Reilly.
 ** 
 ** Do not use this class where a simple mutex will do. There is more overhead
 ** involved to prevent deadlocks and to support safe usage requirements.
 **/
class cond_variable: public mutex
{
	private:
		pthread_cond_t mycv;
		pthread_t owner; // 0 or current lock owner.
		int waiting; // number of threads waiting
	public:
	
		/// Use this to catch all cond_variable exceptions.
		class general_exception: public base_exception {};
		/// Thrown if the cond_variable can not be destructed.
		class can_not_destruct_exception: public general_exception {};
		/// Thrown by *wait() and *signal if the cond_variable is unlocked on entry.
		class not_locked_exception: public general_exception {};
		/// Thrown when relocking a cond_variable already locked by the calling thread.
		class already_locked_exception: public general_exception {};
		/// Thrown when unlocking cond_variable not locked by the calling thread.
		class not_owner_exception: public general_exception {};
		
		/** Constructs the cond_variable.
		 ** 
		 ** The cond_variable is initialized and left unlocked and ready
		 ** to use.
		 **/
		cond_variable();
		
		/** Destructs the cond_variable;
		 ** 
		 ** If the cond_variable is unlocked and not in a wait state all resources 
		 ** are released. In any other case a cant_destruct_exception is thrown.
		 **/
		virtual ~cond_variable();

		/** Locks the cond_variable.
		 ** 
		 ** cond_variable has an internal mutex that is used to control access to the 
		 ** cond_variable and optionally the data it may be interested in. If the
		 ** cond_variable is not currently held by another thread, this locks it and
		 ** returns immediately to the caller. If the cond_variable is already locked 
		 ** by another thread, this call will block until the cond_variable is 
		 ** unlocked ("becomes available") at which time it will be locked and control
		 ** returned to the caller.
		 ** 
		 ** The cond_variable must be locked before entering any of the *wait()
		 ** or *signal() methods and is of course locked on exit of the same. It is
		 ** atomically unlocked while blocked in the *wait() methods to allow 
		 ** other threads to update the protected data and use the *signal() 
		 ** methods.
		 ** 
		 ** NOTE: There is no time limit on how long this method will block while
		 ** attempting to gain a lock. Therefore, once a thread locks a
		 ** cond_variable, it _MUST_ unlock it when the thread is done with the
		 ** resource the cond_variable is protecting. Failure to do so will almost 
		 ** certainly result in program deadlocks.
		 **/
		virtual void lock();

		/** Unlocks the cond_variable.
		 ** 
		 ** This is the inverse of the cond_variable::lock() method. Just as you
		 ** must call lock sometime before entering *wait() or *signal(), you 
		 ** must call unlock() after exiting those methods as soon as it would be
		 ** safe for other threads to access the cond_variable.
		 **/
		virtual void unlock();

		/** Checks the cond_variable and if available locks it.
		 ** 
		 ** If the cond_variable is locked by another thread this method returns to 
		 ** the caller immediately with a return of "false". If the cond_variable 
		 ** is not locked, this locks it and returns "true" to the caller. If the
		 ** cond_variable is already locked by the calling thread true is returned
		 ** and no other action is taken. In no case will this method block.
		 **/
		virtual bool try_lock();

		/** Waits indefinately until signaled.
		 ** 
		 ** The cond_variable must be locked prior to calling this method. On entry 
		 ** to this method the calling thread blocks and the cond_variable will 
		 ** be unlocked so that other threads can modify the data we're protecting.
		 ** 
		 ** When another thread wakes us up (via a call to signal() or 
		 ** broadcast_signal()) the cond_variable will again be locked and this method
		 ** will return to it's caller. It's the responsiblity of the calling
		 ** thread to call unlock() when it's safe for other threads to modify 
		 ** the data again.
		 ** 
		 ** NOTE: If no thread ever calls this cond_variable's signal() or
		 ** broadcast_signal() methods the calling thread will _never_ wake up.
		 ** 
		 ** WARNING: It is not technically an error for either the wait() or 
		 ** time_wait() methods to return control to the calling thread 
		 ** spuriously, i.e. without having been signalled or timing out. It is 
		 ** therefore the calling thread's responsiblity to insure the conditions
		 ** it was waiting on have actually been met and, if not, take the 
		 ** appropriate action.
		 **/
		void wait();

		/** Waits until signaled or a timeout occurs.
		 ** 
		 ** The cond_variable must be locked prior to calling this method. On entry 
		 ** to this method the calling thread blocks and the cond_variable will 
		 ** be unlocked so that other threads can modify the data we're protecting.
		 ** 
		 ** When another thread wakes us up (via a call to signal() or 
		 ** broadcast_signal() or after the specified number of seconds have 
		 ** elapsed) the cond_variable will again be locked and this method
		 ** will return to it's caller. It's the responsiblity of the calling
		 ** thread to call unlock() when it's safe for other threads to modify 
		 ** the data again.
		 ** 
		 ** WARNING: It is not technically an error for either the wait() or 
		 ** time_wait() methods to return control to the calling thread 
		 ** spuriously, i.e. without having been signalled or timing out. It is 
		 ** therefore the calling thread's responsiblity to insure the conditions
		 ** it was waiting on have actually been met and, if not, take the
		 ** appropriate action.
		 **/
		void timed_wait(const time_t & seconds);

		/** Wakes up one thread blocked in wait() or timed_wait();
		 ** 
		 ** When called and if one or more threads are blocked on this 
		 ** cond_variable's wait() or time_wait(), one of those threads will be 
		 ** released and given the lock on the cond_variable. Only one thread 
		 ** will be released, and if there are more than one thread blocked in 
		 ** this cond_variable's wait() or time_wait() you have no direct control
		 ** over which one will be released. Indeed, for all intents and practical 
		 ** purposes you might as well consider the selection to be random.
		 ** 
		 ** Only works if this cond_variable is actually blocking in wait() or 
		 ** time_wait(). Otherwise a not_waiting_exception is thrown.
		 **/
		void signal();
		
		/** Wakes up all threads blocked in wait() or timed_wait();
		 ** 
		 ** When called and if one or more threads are blocked on this 
		 ** cond_variables's wait() or time_wait(), all threads so blocked will be 
		 ** released and given the lock on this cond_variable in turn. You have
		 ** no direct control over the order in which the blocked thread(s) are 
		 ** allowed to run; for all intents and practical purposes you might as 
		 ** well consider the run order of the released threads to be random.
		 ** 
		 ** Only works if this cond_variable is actually blocking in wait() or 
		 ** time_wait(). Otherwise a not_waiting_exception is thrown.
		 **/
		void broadcast_signal();

		/** Returns the number of threads blocked on this cond_variable.
		 **/
		int num_waiting();
};

/** Safely mutex locks the scope it's instanciated in.
 ** 
 ** scope_lock provides a safe and easy way to control 95% percent of mutex 
 ** usage. Instanciating a scope_lock in a given scope locks the supplied 
 ** mutex until the end of the scope. When the scope_lock is destructed upon
 ** exiting the scope the mutex is unlocked, regardless of how the scope is
 ** exited. Even in the case of unexpected exceptions this class will insure
 ** your mutex is accidently left locked to cause problems later.
 **/
class scope_lock
{
	private:
		mutex * mylock;
	public:
		/** Constructs the scope_lock and locks the supplied mutex.
		 ** 
		 ** lock is a mutex (defined in base_thread.h) and is assumed to be 
		 ** ready to use at the time this constructor is called. The mutex will
		 ** be locked at the return of this method, so it may block until the mutex 
		 ** is available if the mutex is held by another thread at the time of entry.
		 ** 
		 ** NOTE: Once a mutex is used to instanciate a scope_lock it should not
		 ** be accessed or modified in any way within the scope being protected 
		 ** by the scope_lock. Failure to abide by this simple rule may cause
		 ** unexpected deadlocks or exceptions.
		 **/
		scope_lock(mutex & lock)
		{
			mylock = &lock;
			mylock->lock();
		};
		/** Destructs the scope_lock and unlocks the associated mutex.
		 ** 
		 ** You never call this directly, of course. Instead, it's called
		 ** automatically upon exit of the scope the scope_lock was instanciated
		 ** in.
		 **/
		~scope_lock()
		{
			mylock->unlock();
		};
};

/** Unlocks the supplied mutex.
 ** 
 ** Use this function in conjunction with pthread_cleanup_push() and
 ** pthread_cleanup_pop(0) to ensure a mutex is unlocked should the thread
 ** be canceled which owns it. Example (where mylock is a mutex or
 ** cond_variable):
 ** 
 ** scope_lock lock(mylock);
 ** 
 ** pthread_cleanup_push(safe_unlock, &mylock);
 ** 
 ** (something interesting happens here...)
 ** 
 ** pthread_cleanup_pop(0)
 ** 
 ** Sorry I can't hide the pthread_cleanup_* calls and make this neater..
 ** unfortunately the pthread standard requires they appear in the same
 ** lexical block.
**/
void safe_unlock(void * mylock);


} // namespace nrtb

#endif // base_thread_h

