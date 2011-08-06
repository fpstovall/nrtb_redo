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

#include "base_thread.h"
#include <boost/lexical_cast.hpp>
#include <sys/resource.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>

using namespace std;

using boost::lexical_cast;
using std::string;

namespace nrtb
{

void runnable::exit(int value)
{
	pthread_exit(&value);
};

void runnable::set_not_cancelable()
{
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
};

void runnable::set_deferred_cancel()
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
};

void runnable::set_cancel_anytime()
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
};

void runnable::test_cancel()
{
	pthread_testcancel();
};

thread::thread()
{
	alt_run = 0;
	mytid = 0;
	start_detached = false;
};

void * thread::startpoint(void * _this)
{
	thread * Me = (thread *) _this;
	//Me->mypid = getpid();
	// Have we had a runnable object passed?
	if (Me->alt_run)
	{
		// call the run() process from the runnable passed in.
		Me->alt_run->run();
	}
	else
	{
		// call the overridden run() method from our class.
		Me->run();
	};
	return NULL; 
};

thread::thread(runnable & do_this)
{
	if (!is_running())
	{
		alt_run = & do_this;
		start_detached = false;
		mytid = 0;
	}
	else
	{
		throw already_running_exception();
	};
};

thread::~thread()
{
	stop();
};

void thread::run()
{
	throw no_code_exception();
};

void thread::start(runnable & do_this)
{
  alt_run = & do_this;
  start();
};

void thread::start()
{
	if (!is_running())
	{
		int results;
		// Build the attributes we'll use for this
		pthread_attr_t attribs;
		pthread_attr_init(&attribs);
		if (start_detached)
		{
			pthread_attr_setdetachstate(&attribs, PTHREAD_CREATE_DETACHED);
		}
		else
		{
			pthread_attr_setdetachstate(&attribs, PTHREAD_CREATE_JOINABLE);
		};
		// start the thread by using our predefined entry point
		results = pthread_create(&mytid,&attribs,startpoint,this);
		// Could we start? Warning: pthread_create returns false if succesful.
		if (results)
		{
			throw no_start_exception();
		};
	}
	else
	{
		// not good; we are already active!
		throw already_running_exception();
	};
};

void thread::stop()
{
	// If we are not running this is a nop.
	if (is_running())
	{
		// warning: pthread_cancel returns false if successful, true otherwise.
		if (pthread_cancel(mytid))
		{
			// this is the only error pthread_cancel returns...
			throw thread_id_not_found_exception();
		};
		mytid = 0;
	};
};

bool thread::is_running()
{
 	if ((mytid == 0) || (pthread_kill(mytid,0) != 0))
	{
		return false;
	}
	else
	{
		return true;
	};
};

int thread::join()
{
	void ** ret_val;
	if (pthread_join(mytid, ret_val))
	{
		throw not_joinable_exception();
	};
	mytid = 0;
	return 0;
};

void thread::detach(bool yes)
{
	if (yes)
	{
		if (is_running())
		{
			pthread_detach(mytid);
		}
		else
		{
			start_detached = true;
		};
	}
	else
	{
		if (is_running()) 
		{
			throw attr_change_failed_exception();
		}
		else
		{
			start_detached = false;
		};
	};
};

int thread::max_priority(int policy)
{
	return sched_get_priority_max(policy);
};

int thread::min_priority(int policy)
{
	return sched_get_priority_min(policy);
};

int thread::get_priority()
{
	int returnme = 0;
	if (is_running())
	{
		int temp;
		sched_param val;
		int ret = pthread_getschedparam(mytid,&temp,&val);
		if(ret != 0)
		{
			cerr << "\n" << __FILE__ << "::" << __FUNCTION__
				<< ": pthread_getschedparam(" << mytid
				<< ",,) returned " << ret << "!" << endl;
			switch (ret)
			{
/*
	backed out hte ESRCH check because I can not find where ESRCH
	is defined in the 2.6 kernel include files!

				case ESRCH : 
				{
					thread_id_not_found_exception e;
					e.store("thread::get_priority()");
					throw e;
				}
*/				default :
				{
					general_exception e;
					e.store("thread::get_priority()");
					throw e;
				}
			};
		};
		returnme = val.sched_priority;
	}
	else
	{
		not_running_exception e;
		e.store("thread:get_priority()");
		throw e;
	};
	return returnme;
};

void thread::set_priority(const int pri, const int policy)
{
	if (!is_running())
	{
		not_running_exception e;
		e.store("thread::set_priority()");
		throw e;
	};
	sched_param val;
	val.sched_priority = pri;
	if (pthread_setschedparam(mytid,policy,&val))
	{
		attr_change_failed_exception e;
		string message = "thread::set_priority(" 
			+ lexical_cast<string>(pri)
			+ ")";
		e.store(message);
		throw e;
	};
};

void thread::yield()
{
	timespec t;
	t.tv_sec = 0;
	t.tv_nsec = 0;
	nanosleep(&t,&t);
};

mutex::mutex()
{
	pthread_mutexattr_t attrib;
	pthread_mutexattr_init(&attrib);
	#ifdef linux_pthreads
		pthread_mutexattr_setkind_np(&attrib,PTHREAD_MUTEX_ERRORCHECK_NP);
	#endif
	pthread_mutex_init(&mymid,&attrib);
//	owner = 0;
};

mutex::~mutex()
{
	if (pthread_mutex_destroy(&mymid))
	{
		throw can_not_destruct_exception();
	};
};

void mutex::lock()
{
	if (pthread_mutex_lock(&mymid))
	{
		throw already_locked_exception();
	};
};

void mutex::unlock()
{
	if (pthread_mutex_unlock(&mymid))
	{
		throw not_owner_exception();
	};
};

bool mutex::try_lock()
{
	if (pthread_mutex_trylock(&mymid))
	{
		return false;
	}
	else
	{
		return true;
	};
};

cond_variable::cond_variable()
	: mutex()
{
	pthread_cond_init(&mycv,NULL);
	waiting = 0;
	owner = 0;
};

cond_variable::~cond_variable()
{
	try
	{
		// lock the mutex before we do anything here.
		// is there anyone waiting on this cond_variable?
		if (!try_lock() || (waiting > 0))
		{	
			// not good, there are others waiting on us or we are locked.
			can_not_destruct_exception e;
			e.store(lexical_cast<string>(waiting));
			unlock();
			throw e;
		}
		else
		{
			pthread_cond_destroy(&mycv);
		};
		// unlock before we leave.
		unlock();
	}
	catch (mutex::general_exception & e)
	{
		throw e;
	};
};

void cond_variable::lock()
{
	if (pthread_equal(pthread_self(),owner))
	{
		throw already_locked_exception();
	}
	else
	{
		mutex::lock();
		owner = pthread_self();
	};
};

void cond_variable::unlock()
{
	if (pthread_equal(pthread_self(),owner))
	{
		owner = 0;
		mutex::unlock();
	}
	else
	{
		not_owner_exception e;
		e.store(lexical_cast<string>(owner));
		throw e;
	};
};

bool cond_variable::try_lock()
{
	if (pthread_equal(pthread_self(),owner))
	{
		return true;
	}
	else if (mutex::try_lock())
	{
		owner = pthread_self();
		return true;
	}
	else
	{
		return false;
	};	
};

void cond_variable::wait()
{
	if (pthread_equal(pthread_self(),owner))
	{
		waiting++;
		pthread_cond_wait(&mycv,&mymid);
		waiting--;
		owner = pthread_self();
	}
	else
	{
		throw not_locked_exception();
	};
};

void cond_variable::timed_wait(const time_t & seconds)
{
	if (pthread_equal(pthread_self(),owner))
	{
		waiting++;
		time_t endtime = time(NULL) + seconds;
		timespec delay;
		delay.tv_sec = endtime;
		delay.tv_nsec = 0;
		pthread_cond_timedwait(&mycv,&mymid,&delay);
		// pthread_cond_timewait can return a couple of error conditions, 
		// but we don't care since we will treat any return the same way.
		waiting--;
		owner = pthread_self();
	}
	else
	{
		throw not_locked_exception();
	};
};

void cond_variable::signal()
{
	if (pthread_equal(pthread_self(),owner))
	{
		pthread_cond_signal(&mycv);
	}
	else
	{
		throw not_locked_exception();
	};
};

void cond_variable::broadcast_signal()
{
	if (pthread_equal(pthread_self(),owner))
	{
		pthread_cond_broadcast(&mycv);
	}
	else
	{
		throw not_locked_exception();
	};
};

int cond_variable::num_waiting()
{	
	lock();
	int i = waiting;
	unlock();
	return i;
};

void safe_unlock(void * mylock)
{
	try { static_cast<mutex *>(mylock)->unlock(); }
	catch (...) {};
};


} // namespace nrtb
