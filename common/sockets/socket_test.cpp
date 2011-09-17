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

#include <iostream>
#include <sstream>
#include <string>
#include <boost/random.hpp>
#include "base_socket.h"
#include <boost/shared_ptr.hpp>

using namespace nrtb;
using namespace std;

class myserver: public tcp_server_socket_factory
{
public:
	int hits;
	int errors;

	// constructor
	myserver(const string & a, const unsigned short int & b) 
		: tcp_server_socket_factory(a,b)
	{
		// Don't need to lock here because we know the 
		// listener thread is not running.
		hits = 0;
		errors = 0;
	};

protected:
	// on_accept() is called on each connection.
	bool on_accept()
	{
		try
		{
			// just return what we've recieved.
			string msg = connect_sock->getln();
			connect_sock->put(msg);
			// Update our hit count. 
			hits++;
		}
		catch (base_exception & e)
		{
		  errors++;
		  cerr << "server Caught " << e.what() << endl;
		}
		catch (...)
		{
		  errors++;
		  cerr << "Unexpected error in on_accept()" << endl;
		};
		if (hits > 99) 
		  return false;
		else
		  return true;
	};
};

string transceiver(const string address, const string sendme)
{
  string returnme;
  tcp_socket sender;
  sender.connect(address);
  sender.put(sendme);
  returnme = sender.getln();
  sender.close();//cerr << "tc>> sock closed" << endl;
  return returnme;
};

string address = "127.0.0.1:";
int port_base = 17000;

int main()
{
  int er_count = 0;
  //set up our port and address
  boost::mt19937 rng;
  rng.seed(time(0));
  boost::uniform_int<> r(0,1000);
  stringstream s;
  s << address << port_base + r(rng);
  address = s.str();
  cout << "Using " << address << endl;
  
  myserver test_server(address,5);

  try
  {
	// start the receiver/server
	test_server.start_listen();
	usleep(5e5);
	
	// Send test messages
	for (int i = 0; i < 100; i++)
	{
	  stringstream msg;
	  msg << "test message " << i << "\r";
	  string checkme = msg.str();
	  string returned = transceiver(address, checkme);
	  if (returned != checkme)
	  {
		er_count++;
	  };
	  cout << returned.substr(0,returned.size()-1) << ": " 
		<< ((returned == checkme) ? "Passed" : "Failed")
		<< endl;
	};
  }
  catch (myserver::bind_failure_exception)
  {
	  cout << "Could not bind port" << endl;
  }
  catch (myserver::mem_exhasted_exception)
  {
	  cout << "myserver reports out of memory." << endl;
  }
  catch (myserver::listen_terminated_exception)
  {
	  cout << "Listener terminated unexpectedly." << endl;
  }
  catch (myserver::on_accept_bound_exception)
  {
	  cout << "myserver::on_accept() seems bound." << endl;
  }
  catch (tcp_socket::bad_connect_exception & e)
  {
	  cout << "A bad_connect_exception was thrown.\n" 
		<< e.comment() << endl;
  }
  catch (tcp_socket::not_open_exception & e)
  {
	  cout << "A tcp not open exception was caught.\n" 
		<< e.comment() << endl;
  }
  catch (tcp_socket::close_exception & e)
  {
	  cout << "A close_exception was caught.\n" 
		<< e.comment() << endl;
  }
  catch (tcp_socket::overrun_exception & e)
  {
	  cout << "An overrun_exception was caught.\n" 
		<< e.comment() << endl;
  }
  catch (tcp_socket::buffer_full_exception & e)
  {
	  cout << "A buffer_full_exception was caught.\n" 
		<< e.comment() << endl;
  }
  catch (tcp_socket::general_exception & e)
  {
	  cout << "A tcp_socket exception was caught.\n" 
		<< e.comment() << endl;
  }
  catch (exception & e)
  {
	  cout << "A unexpected " << e.what() << " exception was caught." << endl;
  };

  // final check.
  if (test_server.hits != 100)
  {
	er_count++;
	cout << "Server does not report the proper number of hits.\n"
	  << "\tExpected 100, got " << test_server.hits 
	  << endl;
  };
  cout << "=========== tcp_socket test complete =============" << endl;
  
  return er_count;
};



























