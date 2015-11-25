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

struct lvar
{
  triplet location;
  triplet velocity;
  triplet attitude;
  triplet rotation;
  void load(string data)
  {
    string junk;
    stringstream s(data);
    s >> junk >> junk // "bot lvar"
      >> location >> velocity >> attitude >> rotation;
  }
};

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
  float zone(config.get<float>("zone",1000));
  float turnslop(config.get<float>("turnslop",0.6));

  // start com handler.
  async_com_handler sim(server_addr);
  
  try
  {
    string sys, verb;
    triplet center;
    lvar start;
    lvar current;
    // get bot ack.
    cout << "\n" << sim.get() << endl;
    
    // get and record initial conditions
    sim.put("bot lvar");
    start.load(sim.get());
    current = start;
    (seek.z < 0) ? center = start.location : center = seek;
    
    // report starting status
    cout << "landed at " << start.location << endl
      << "zone center : " << center << endl
      << "  zone size : " << zone << endl
      << "      range : " << current.location.range(center) 
      << "  turn slop : " << turnslop
      << endl;
      
    sim.put("drive power 25");

    const int run=1;
    const int stopping=2;
    const int turning=3;
    const int tofar=4;
    int state(run); 
    triplet wayhome;
    long long unsigned counter(0);
    
    // working loop
    while (true) // literally loop till we die....
    {
      // get current conditions
      sim.put("bot lvar");
      current.load(sim.get());
      
      if ((++counter % 50) == 0)
      {
        cout << counter 
          << "\n\t" << current.location
          << "\n\t" << current.velocity
          << "\n\t" << current.location.range(center)
          << endl;
      };
      
      // check conditions
      switch (state)
      {
        case run : 
        {
          if (current.location.range(center) > zone) state = tofar;
          break;
        }
        case tofar :
        {
          cout << "Too far from base." << endl;
          sim.put("drive power 0");
          sim.put("drive brake 100");
          state = stopping;
          break;
        }
        case stopping : 
        {
          if (current.velocity.magnatude() < 0.01)
          {
            cout << "Stopped" << endl;
            sim.put("drive brake 0");
            // where are we relative to center?
            wayhome = (center - current.location).to_polar();
            // start turn
            sim.put("drive turn 25");
            // set state
            state = turning;
          }
          break;
        }
        case turning : 
        {
          float diff = fabs(current.attitude.x - wayhome.y);
          // check to see if we have the heading yet.
          if (diff < turnslop)
          {
            cout << "Turn complete." << endl;
            // -- stop turn
            sim.put("drive turn 0");
            // -- start drive
            sim.put("drive power 20");
            // -- set status to run
            state = run;
          }
          break;
        }
      };
      int t(1e6/50);
      std::chrono::microseconds st(t);
      this_thread::sleep_for(st);
    };
  }
  catch (...)
  {};
  
};



























