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

#include <iostream>
#include <sstream>
#include <string>
#include <future>
#include <typeinfo>
#include <common.h>
#include "base_socket.h"
#include <sys/time.h>
#include <time.h>

using namespace nrtb;
using namespace std;

int request_processor(tcp_server_socket_factory & server)
{
  int hits {0};
  bool done {false};
  while (!done)
  {
    try
    {
      auto sock = server.get_sock();
      // just return what we've recieved.
      string msg = sock->getln();
      sock->put(msg);
      // Update our hit count.
      hits++;
    }
    catch (tcp_server_socket_factory::queue_not_ready)
    {
      done = true;
    }
    catch (exception & e)
    {
      cout << "ReqProc recieved " 
	<< typeid(e).name()
	<< endl;
      done = true;
    };
  }; 
  cout << "Request processor shut down." << endl;
  return hits;
};

string transceiver(const string address, const string sendme)
{
  string returnme;
  tcp_socket sender;
  sender.connect(address);
  sender.put(sendme);
  returnme = sender.getln();
  sender.close();
  return returnme;
};

string address = "127.0.0.1:";
int port_base = 17000;

int main()
{
  cout << "=========== tcp_socket and server test ============="
    << endl;

  int er_count {0};
  int hits {0};
  timeval startt;
  gettimeofday(&startt,0);
  int offset = startt.tv_usec % 1000;
  stringstream s;
  s << address << port_base + offset;
  address = s.str();
  cout << "Using " << address << endl;
  
  tcp_server_socket_factory test_server(address);

  try
  {
    // start the receiver/server
    test_server.start_listen();
    cout << "test_server ready." << endl;
    auto rp_out = 
      async(launch::async,request_processor,std::ref(test_server));
    cout << "Request processor attached." << endl;
    
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
    };
    test_server.stop_listen();
    hits = rp_out.get();
  }
  catch (tcp_server_socket_factory::general_exception & e)
  {
    cout << "Server exception " 
      << typeid(e).name() << " was caught."
      << "\n\tComment: " << e.comment() 
      << endl;
  }
  catch (tcp_socket::general_exception & e)
  {
    cout << "Socket exception "
      << typeid(e).name() << " was caught.\n"
      << "\tComment: " << e.comment() << endl;
  }
  catch (exception & e)
  {
    cout << "A unexpected " << e.what()
      << " exception was caught." << endl;
  };

  // test server status billboard
  cout << "Test Server Results:"
    << "\n\tAccepted:  " << test_server.accepted()
    << "\n\tProcessed: " << test_server.processed()
    << "\n\tAvailable: " << test_server.available()
    << "\n\tDiscarded: " << test_server.discarded()
    << "\n\tLast Fault: " << test_server.last_fault()
    << endl;
  // final check.
  if (hits != test_server.accepted() 
    or hits != test_server.processed()
    or test_server.discarded() != 0  )
  {
    er_count++;
    cout << "Server does not report the proper number of hits.\n"
      << "\tExpected 100, got " << hits
      << endl;
  };
  cout << "=========== tcp_socket and server test complete ============="
    << endl;
  
  return er_count;
};



























