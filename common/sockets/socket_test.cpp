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
#include "base_socket.h"
#include <base_thread.h>

using namespace nrtb;
using namespace std;

class myserver: public tcp_server_socket_factory
{
	private:
		int hits;
		int errors;
		mutex data;
	protected:
		// on_accept() is called on each connection.
		void on_accept()
		{
			try
			{
				// just return what we've recieved.
				string msg = connect_sock->getln();
				connect_sock->put(msg);
				// Close the socket.
				connect_sock->close();
				// delete the tcp_socket (it was new'd before we were called)
				delete connect_sock;
				// Update our hit count. 
				scope_lock lock(data);
				hits++;
			}
			catch (base_exception & e)
			{
			  errors++;
			  cerr << "Caught " << e.what() << endl;
			}
			catch (...)
			{
			  errors++;
			  cerr << "Unexpected error in on_accept()" << std::endl;
			};
		};
	public:
		// constructor
		myserver(const string & a, const unsigned short int & b) 
			: tcp_server_socket_factory(a,b)
		{
			// Don't need to lock here because we know the 
			// listener thread is not running.
			hits = 0;
		};
		// hit count reporter.
		int hit_count() 
		{
			scope_lock lock(data); 
			return hits; 
		};
};

string transceiver(const string address, const string sendme)
{
  string returnme;
  tcp_socket sender;
  sender.connect(address);
  sender.put(sendme);
  returnme = sender.getln();
  return returnme;
};

const string address = "localhost:17000";

int main()
{
  int er_count = 0;
  myserver test_server(address,5);
  
  try
  {
	// start the receiver/server
	test_server.start_listen();
	
	// Send test messages
	for (int i = 0; i < 1000; i++)
	{
	  stringstream msg;
	  msg << "test message " << i;
	  string checkme = msg.str();
	  msg << "\r";
	  string returned = transceiver(address, msg.str());
	  if (returned != checkme)
	  {
		er_count++;
	  };
	  cout << returned << ": " 
		<< ((returned == checkme) ? "Passed" : "Failed")
		<< endl;
	};
	test_server.stop_listen();
	usleep(5000);
	if (test_server.listening())
	{  
	  er_count++;
	  cout << "Server failed to stop. " << endl;
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
  catch (tcp_socket::general_exception)
  {
	  cout << "A tcp_socket exception was caught." << endl;
  }
  catch (exception & e)
  {
	  cout << "A unexpected " << e.what() << " exception was caught." << endl;
  };

  // final check.
  if (test_server.hit_count() != 1000)
  {
	er_count++;
	cout << "Server does not report the proper number of hits."
	  << endl;
  };
  cout << "=========== tcp_socket test complete =============" << endl;
  
  return er_count;
};



























