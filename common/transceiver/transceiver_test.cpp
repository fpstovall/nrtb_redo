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
typedef boost::shared_ptr<my_msg> my_msgp;
typedef transceiver<my_msg,my_msg> linkt;

int er_count = 0;

class server_work_thread: public runnable
{
public:
  
  tcp_socketp sock;
  unsigned long long last_inbound;
  
  void run()
  {
	set_cancel_anytime();
	linkt link(sock);
	while (sock->status() == tcp_socket::sock_connect)
	{
	  try 
	  {
		linkt::out_ptr inbound = link.get();
		last_inbound = inbound->msg_uid();
		cout << "\tReceived #" << last_inbound << endl;
		if (last_inbound == 99)
		  exit(0);
	  }
	  catch (linkt::general_exception & e)
	  {
		cerr << "Server work thread caught " << e.what()
		  << "\n\tComment: " << e.comment() << endl;
		er_count++;
	  }
	  catch (tcp_socket::general_exception & e)
	  {
		cerr << "Server work thread caught " << e.what()
		  << "\n\tComment: " << e.comment() << endl;
		er_count++;
	  }
	  catch (std::exception & e)
	  {
		cerr << "Server work thread caught " << e.what() 
		  << endl;
		er_count++;
	  };
	};
  };
};

thread process;
server_work_thread task;
  
class listener: public tcp_server_socket_factory
{
public:
  listener(const string & add, const int & back)
   : tcp_server_socket_factory(add, back) {};
  
  void on_accept()
  {
	if (!process.is_running())
	{
	  task.last_inbound = 0;
	  task.sock.reset(connect_sock);
	  process.start(task);
	  cout << "server thread running." << endl;
	}
	else
	{
	  tcp_socketp s(connect_sock);
	  s->close();
	  cerr << "Multiple attempts to connect to server" 
		<< endl;
	};
  };
};

string address = "127.0.0.1:";
int port_base = 12334;

int main()
{
  setup_global_logging("transceiver.log");
  
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
  usleep(5e5);

  // set up our sender
  tcp_socketp sock(new tcp_socket);
  sock->connect(address);
  linkt sender(sock);

  // Let's send a few things.
  for (int i=0; i<100; i++)
  {
	my_msgp msg(new my_msg);
	sender.send(msg);
	cout << "Sent " << msg->msg_uid() << endl;
	usleep(1e4);
  };
  
  usleep(5e5);
  return er_count;
};