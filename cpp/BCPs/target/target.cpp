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

#include <fstream>
#include <common.h>
#include <logger.h>
#include <confreader.h>
#include <base_socket.h>
#include <hires_timer.h>
#include <linear_queue.h>
#include <triad.h>

using namespace nrtb;
using namespace std;

typedef triad<float> triplet;


class async_com_handler
{
public:
  async_com_handler(std::string addr)
    : address(addr), active(false)
  {
    worker_thread = std::thread(async_com_handler::worker, std::ref(*this));
  };
  ~async_com_handler()
  {
    if (active) try {close();} catch (...) {};
  };
  std::string get()
  {
    return inbound.pop();
  };
  void put(std::string msg)
  {
    sock.put(msg+"\r");
  };
  void close()
  {
    active = false;
    sock.close();
    if (worker_thread.joinable()) worker_thread.join();
  };
private:
  linear_queue<std::string> inbound;
  std::string address;
  std::atomic<bool> active;
  tcp_socket sock;
  std::thread worker_thread;
  static void worker(async_com_handler & a)
  {
    a.active = true;
    try
    {
      // Make the socket connection.
      a.sock.connect(a.address);
      // loop
      while (a.active)
      {
        try
        {
          a.inbound.push(gsub(a.sock.getln("\r",0,1),"\r",""));
        }
        catch (tcp_socket::timeout_exception e) {};
      };
    }
    catch (tcp_socket::not_open_exception & e)
    {
      // nop... this is normal af the end.
    }
    catch (base_exception & e) 
    {
      cerr << "Exception at " << __FILE__ << ":" << __LINE__ << endl;
      cerr << e.what() << "\n" << e.comment() << endl;
    }
    catch (std::exception & e)
    {
      cerr << "Exception at " << __FILE__ << ":" << __LINE__ << endl;
      cerr << e.what() << endl;
    }
    catch (...) 
    {
      cerr << "Unknown error at " << __FILE__ << ":" << __LINE__ << endl;
    };
    // close out the connection
    try {a.sock.close();} catch (...) {};
    a.active = false;
  };
};


int main(int argc, char * argv[])
{
  hirez_timer runtime();
  // load the global configuration
  conf_reader config;
  config.read(argc, argv, "target.conf");
  
  // display config choices.
  cout << endl;
  for (auto v: config) cout << v.first << "=" << v.second << endl;
  
  // get the configuration data
  string server_addr(config.get<string>("server","127.0.0.1:64500"));
  triplet seek(config.get<triplet>("seek",triplet(0,0,-1)));
  int zone(config.get<int>("zone",1000));

  // start com handler.
  async_com_handler sim(server_addr);
  
  try
  {
    string sys, verb; 
    // get bot ack.
    cout << "\n" << sim.get() << endl;
    
    // get and publish our curent location.
    sim.put("bot lvar");
    // get bot ack.
    cout << sim.get() << endl;
    
      
    
  }
  catch (...)
  {};
  
};



























