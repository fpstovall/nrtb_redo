#include <work_queue_thread.h>
#include <free_thread.h>
#include <list>

namespace NRTB
{

/** A work_queue_thread with a global control queue.
 ** 
 ** First, read the documentation on work_queue_thread if you are not
 ** comfortable with it. A controlled_work_queue_thread differs from a
 ** work_queue_thread in that it mechanism for sending "control" messages
 ** to all of the threads servicing a given queue.
 ** 
 ** Control messages are sent once, but are presented to each worker thread
 ** servicing the queue. They are processed on a priority basis, before
 ** messages in the normal work queue, and are always processed in the
 ** order they were sent by each thread. Control messages may be a
 ** completely different data type than the work queue messages and are
 ** proceessed by a different method.
 ** 
 ** In addition to overridding the do_work() abstract method, you must
 ** override the do_control() method to define the actions you'll take on
 ** any given control message. In addition to the new required template
 ** parameter cltmsg, there is a new optional ctl_seq parameter that allows
 ** you to override the control queue type; it's subject to the same rules
 ** and restrictions as noted for the seq parameter, save that it must
 ** store control messages.
 ** 
 ** The default thread type for controlled_work_queue_threads is
 ** ricklib::free_thread. This is a good choice, since the control queue
 ** allows easy manipulalation of free running threads. However, this
 ** template should work fine with any clean descendent of ricklib::thread.
 ** 
 ** NOTE: run() and closeout() provide critical functionality for the
 ** automatic maintainence of the thread list and control queue. Don't
 ** override them unless you grok them completely. In practice, I don't
 ** expect you'll ever need to.
**/
template <class workpacket, class ctlmsg,
	class threadclass=ricklib::free_thread, 
	int q_uid = 0, class seq = std::queue<workpacket>,
	class ctl_seq = std::queue<ctlmsg> > 
class controlled_work_queue_thread
	: public work_queue_thread<workpacket,threadclass,q_uid,seq>
{
	public:
		/** Sends a control message to all the threads.
		 ** 
		 ** The ctlmsg is copied into the control queue of all the 
		 ** worker threads operating off of the same work queue.  
		**/
		static void control(const ctlmsg & msg);
	protected:
		typedef work_queue_thread<workpacket,threadclass,q_uid,seq> parent_type; 
		// static control variables.
		typedef controlled_work_queue_thread * cwqt_ptr;
		typedef std::list<cwqt_ptr> tlist_type;
		static tlist_type tlist;
		// local control variables;
		ctl_seq ctl_queue;
		bool control_pending;
		ctlmsg my_ctl;
		// methods
		/** Provides control message handling.
		 ** 
		 ** You must override this method to provide the control 
		 ** message processing logic. On entry, the control message
		 ** will be in the my_ctl data element. You are free to take
		 ** any action here, including terminating the thread run.
		**/
		virtual void do_control() = 0;
		/** called when the thread had data to process.
		 ** 
		 ** Override this to do the work you need to do. You'll find the data
		 ** in my_data, which will be of whatever type you specified to make
		 ** the template. This is where you'll actually get the work on the
		 ** workpacket done.
		**/
		virtual void do_work() = 0;
		// must override park() and run()
		virtual void park();
		virtual void run();
		// insures thread is pulled from tlist
		static void closeout(void * arg);
	public:
		// used to add control message to the private queue
		// it is not intended to be called directly by user code.
		// Due to the lack of mutex locking here, calling this
		// can result in _EVIL_ behaviour.
		virtual void add_to_control(const ctlmsg & msg);
};

template <class workpacket, class ctlmsg,
	class threadclass, int q_uid, class seq, class ctl_seq>
typename controlled_work_queue_thread<workpacket,ctlmsg,threadclass,
	q_uid,seq,ctl_seq>::tlist_type
	controlled_work_queue_thread<workpacket,ctlmsg,threadclass,
	q_uid,seq,ctl_seq>::tlist;

template <class workpacket, class ctlmsg,
	class threadclass, int q_uid, class seq, class ctl_seq>
void controlled_work_queue_thread<workpacket,ctlmsg,threadclass,
	q_uid,seq,ctl_seq>::control(const ctlmsg & msg)
{
	scope_lock lock(parent_type::latch);
	typename tlist_type::iterator it = tlist.begin();
	typename tlist_type::iterator end = tlist.end();
	while (it != end)
	{
		if (*it) 
		{
			(*it)->add_to_control(msg);
		};
		it++;
	};
	parent_type::latch.broadcast_signal();
};

template <class workpacket, class ctlmsg,
	class threadclass, int q_uid , class seq, class ctl_seq>
void controlled_work_queue_thread<workpacket,ctlmsg,threadclass,
	q_uid,seq,ctl_seq>::park()
{
	scope_lock lock(parent_type::latch);
	pthread_cleanup_push(ricklib::safe_unlock,&parent_type::latch);
	// increment the number of threads available.
	parent_type::available++;
	control_pending = false;
	// wait until there is data to work with.
	// NOTE: this latch.wait() is a thread cancelation point.
	while (parent_type::pending.empty() && ctl_queue.empty()) {parent_type::latch.wait(); };
	// take the message as our own and clear the buffer.
	if (ctl_queue.empty())
	{
		parent_type::my_data = parent_type::pending.front();
		parent_type::pending.pop();
	}
	else
	{
		my_ctl = ctl_queue.front();
		ctl_queue.pop();
		control_pending = true;
	};
	// decrement the number of threads available.
	parent_type::available--;
	pthread_cleanup_pop(0);
};
	
template <class workpacket, class ctlmsg,
	class threadclass, int q_uid, class seq, class ctl_seq>
void controlled_work_queue_thread<workpacket,ctlmsg,threadclass,
	q_uid,seq,ctl_seq>::run()
{
	{
		scope_lock lock(parent_type::latch);
		tlist.push_front(this);
	}
	pthread_cleanup_push(controlled_work_queue_thread::closeout,this);
	while (true)
	{
		try
		{
			park(); // this is a cancellation point
			if (control_pending)
			{
				do_control();
			}
			else
			{	
				do_work();
			};
		}
		catch (std::exception & e)
		{
			std::clog << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__
				<< ": WARNING!! unexpected exception \"" << e.what() << "\" caught." << std::endl;
		};
	};
	pthread_cleanup_pop(1);
};

template <class workpacket, class ctlmsg,
	class threadclass, int q_uid, class seq, class ctl_seq>
void controlled_work_queue_thread<workpacket,ctlmsg,threadclass,
	q_uid,seq,ctl_seq>::add_to_control(const ctlmsg & msg)
{
	// can not lock latch here, because it's locked by control();
	// ___NEVER___ call this method directly
	ctl_queue.push(msg);
};

template <class workpacket, class ctlmsg,
	class threadclass, int q_uid, class seq, class ctl_seq>
void controlled_work_queue_thread<workpacket,ctlmsg,threadclass,
	q_uid,seq,ctl_seq>::closeout(void * arg)
{
	scope_lock lock(parent_type::latch);
	tlist.remove(static_cast<cwqt_ptr>(arg));
};

} // namespace NRTB
