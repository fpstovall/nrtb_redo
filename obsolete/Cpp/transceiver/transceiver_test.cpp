/***********************************************
 This file is part of the NRTB project (https://github.com/fpstovall/nrtb_alpha).
 
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
 
#include <string>
#include <iostream>
#include <base_thread.h>
#include <base_socket.h>
#include "transceiver.h"
#include <log_setup.h>
#include <sim_to_db_wrapper.pb.h>
#include <boost/random.hpp>

using namespace nrtb;
using namespace std;

typedef nrtb_msg::sim_to_db my_msg;
typedef transceiver<my_msg,my_msg> linkt;

class safe_counter
{
private:
  mutex data_lock;
  int er_count;

public:
  
  safe_counter() { er_count = 0; };

  ~safe_counter() {};

  void inc()
  {
	  scope_lock lock(data_lock);
	  er_count++;
  };

  int operator ()()
  {
	  scope_lock lock(data_lock);
	  return er_count;
  };
};

safe_counter er_count;

class server_work_thread: public thread
{
public:
  
  tcp_socket_p sock;
  unsigned long long last_inbound;

  server_work_thread()
  {
    cout << "Creating server_work_thread." << endl;
    last_inbound = 0;
  }
  
  ~server_work_thread()
  {
	  cout << "Destructing server_work_thread" << endl;
  };
  
  void run()
  {
	  set_cancel_anytime();
	  linkt link(sock);
	  while (link.is_connected())
	  {
	    try 
	    {
		    linkt::in_ptr inbound = link.get();
		    last_inbound = inbound->msg_uid();
		    link.send(inbound);
		    if (last_inbound == 99)
		    {
		      cout << "Receiver thread closing." << endl;
		      exit(0);
		    };
      }
      catch (linkt::general_exception & e)
      {
	      cerr << "Server work thread caught " << e.what()
	        << "\n\tComment: " << e.comment() << endl;
	      er_count.inc();;
      }
      catch (tcp_socket::general_exception & e)
      {
	      cerr << "Server work thread caught " << e.what()
	        << "\n\tComment: " << e.comment() << endl;
	      er_count.inc();;
      }
      catch (std::exception & e)
      {
	      cerr << "Server work thread caught " << e.what() 
	        << endl;
	      er_count.inc();;
	    };
	  };
  };
};

class listener: public tcp_server_socket_factory
{
public:

  std::unique_ptr<server_work_thread> task;

  listener(const string & add, const int & back)
   : tcp_server_socket_factory(add, back)
  {
    cout << "Listener constructed." << endl;
  };
   
  ~listener()
  {
	  cout << "Destructing listener" << endl;
	  // check to see if the listener is still up.
	  try
	  {
	    if (listening())
      {
        cerr << "  Listener is still running...";
        stop_listen();
        cerr << " shutdown is complete." << endl;
      };
	  }
	  catch (...)
	  {
	    cerr << "  Presuming listener is down." << endl;
	  };
    if (!task)
    {
      cerr << "  Task is not allocated. " << endl;
    }
	  // check to see if task is still running and display
	  // a warning if it is.
	  if (task and (task->is_running()))
	  {
	    cerr << "WARNING: Worker is still running!!" << endl;
	    task->stop();
	    task->join();
	    cerr << "Worker thread shutdown is complete." << endl;
	  };
  };
  
  bool on_accept()
  {
	  if (!task)
	  {
      cout << "In listener::on_accept()" << endl;
      task.reset(new server_work_thread);
      task->sock = std::move(connect_sock);
      task->start();
      cout << "server thread started." << endl;
	    // shutdown the listener thead.. our work is done here.
	    return false;
	  }
	  else
	  {
	    connect_sock->close();
	    cerr << "Multiple attempts to connect to server" 
		  << endl;
	  };
  };
};

string address = "127.0.0.1:";
int port_base = 14334;

int main()
{
  setup_global_logging("transceiver.log");

  try
  {
    //set up our port and address
    boost::mt19937 rng;
    rng.seed(time(0));
    boost::uniform_int<> r(0,1000);
    stringstream s;
    s << address << port_base + r(rng);
    address = s.str();
    cout << "Using " << address << endl;

    // kick off the listener thread.
    listener server(address,5);
    server.start_listen();
    cout << "Listener thread is ready." << endl;

    // set up our sender
    tcp_socket_p sock(new tcp_socket);
    int trycount = 0;
    while (sock->status() != tcp_socket::sock_connect)
    {
      try
      {
        sock->connect(address);
      }
      catch (tcp_socket::bad_connect_exception & e)
      {
        trycount++;
        if (trycount > 99)
        {
          cerr << "Too many connect failures for the sender socket."
            << endl;
          throw e;
        };  
        usleep(1e4);
      };
    }
    cout << "sender socket is connected to listener" << endl;
    linkt sender(sock);
    cout << "Sender transciever is ready to use." << endl;

    // Let's send a few things.
    for (int i=0; i<100; i++)
    {
      linkt::out_ptr msg(new my_msg);
      sender.send(msg);
      cout << "Sent " << msg->msg_uid() << ", ";
      msg = sender.get();
      cout << "good return." << endl;
    };
    usleep(1e4);
  }
  catch (tcp_server_socket_factory::bind_failure_exception & e)
  {
    cout << "Listener could not bind " << e.comment() << endl;
    er_count.inc();
  }
  catch (...)
  {
	  cout << "exception caught during test." << endl;
	  er_count.inc();
  };

  int faults = er_count(); 
  if (faults)
  {
	  cout << "========== ** There were " << faults
	    << " errors logged. =========" << endl; 
  }
  else
  {
	  cout << "========= nrtb::transceiver test complete.=========" 
	    << endl;
  };

  return faults;
};
