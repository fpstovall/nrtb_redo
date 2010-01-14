/* work queue thread test program 

	TODO: Needs to be adapted to boost::theads.

*/

//#include <free_thread.h>
#include <work_queue_thread.h>
#include <controlled_wqt.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <queue>

using namespace NRTB;
using namespace std;

mutex c_lock;

class worker : public work_queue_thread<int,free_thread>
{
	public:
		worker()
		{
			scope_lock lock(wlock);
			iam = count++;
			start();
			scope_lock clock(c_lock);
			cout << "worker " << iam << " started" << std::endl;
		};
		~worker()
		{
			scope_lock lock(c_lock);
			cout << "worker " << iam << " is being deleted." << std::endl;
		};
	protected:

		static mutex wlock;
		static int count;
		int iam;
			
		virtual void do_work()
		{
			if (my_data >= 0)
			{
				timespec delaytime;
				delaytime.tv_sec = 0;
				delaytime.tv_nsec = my_data * 1000;
				nanosleep(&delaytime,NULL);
				{
					scope_lock lock(c_lock);
					cout << "\tworker " << iam 
						<< " reports packet " << my_data << std::endl;
				}
			}
			else
			{
				pthread_exit(0);
			};
		};
};

mutex worker::wlock;
int worker::count = 0;

class workera : public controlled_work_queue_thread<int,int,free_thread,1>
{
	public:
		workera()
		{
			scope_lock lock(wlock);
			iam = count++;
			start();
			scope_lock clock(c_lock);
			cout << "workera " << iam << " started" << std::endl;
		};
		~workera()
		{
			scope_lock lock(c_lock);
			cout << "workera " << iam << " is being deleted." << std::endl;
		};
	protected:

		static mutex wlock;
		static int count;
		int iam;
			
		virtual void do_work()
		{
			timespec delaytime;
			delaytime.tv_sec = 0;
			delaytime.tv_nsec = my_data * 1000;
			nanosleep(&delaytime,NULL);
			{
				scope_lock lock(c_lock);
				cout << "\t\tworkera " << iam 
					<< " reports packet " << my_data << std::endl;
			}
		};

		virtual void do_control()
		{
			switch (my_ctl)
			{
				case 1: pthread_exit(0); break;
				default: 
				{
					scope_lock lock(c_lock);
					cout << "\t\t\tworkera " << iam 
						<< " received control message " << my_ctl
						<< std::endl;
				}
			};
		};
};

mutex workera::wlock;
int workera::count = 0;

int main()
{
	// start five workers
	cout << "*** Starting workers ***" << endl;
	for (int i=0; i < 5; i++)
	{
		new worker;
		new workera;
	};
	cout << "*** sending control message 0 ***" << endl;
	workera::control(0);
	cout << "*** queuing 0-9 (0-90) ***" << endl;	
	for (int i = 0; i < 10; i++) 
	{
		worker::store(i);
		workera::store(i*10);
	};
	cout << "*** sending control message 2 ***" << endl;	
	workera::control(2);
	cout << "*** sending 10 \"-1\" to workers ***" << endl;
	for (int i = 0; i < 10; i++) 
	{
		worker::store(-1);
	};
	cout << "*** two second sleep here ***" << endl;
	sleep(2);
	cout << "*** Shutting down CWQ now. ***" << endl;
	workera::control(1);
	cout << "*** Starting 10 second sleep ***" << endl;
	sleep(10);
	cout << "worker has " << worker::parked() << " parked threads and " 
		<< worker::work_pending() << " elements in the queue." << endl;
	cout << "workera has " << workera::parked() << " parked threads and " 
		<< workera::work_pending() << " elements in the queue." << endl;
};
