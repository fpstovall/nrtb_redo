/***********************************************
 This file is part of the NRTB project (https://github.com/fpstovall/nrtb_beta).
 
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

lvar get_my_state(async_com_handler & s)
{
  lvar returnme;
  s.put("bot lvar");
  returnme.load(s.get());
  return returnme;
};

lvar get_target(async_com_handler & s)
{
  lvar returnme;
  std::string tossme;
  s.put("radar contacts");
  std::stringstream contacts(s.get());
  int count;
  contacts >> tossme >> tossme >> count;
  if (count > 0)
  {
    contacts >> tossme
      >> returnme.location
      >> returnme.velocity;
  }
  else
  {
    returnme.location.x = -1.0;
  };
  return returnme;
};

int main(int argc, char * argv[])
{
  hirez_timer runtime;
  // load the global configuration
  conf_reader config;
  config.read(argc, argv, "kamikaze.conf");
  
  // get the configuration data
  string server_addr(config.get<string>("server","127.0.0.1:64500"));
  
  // start the comm handler.
  async_com_handler sim(server_addr);

  string state = "new";
  
  try
  {
    string sys, verb; 
    lvar current;
    lvar target;
    int counter(0);

    // get bot ack.
    cout << "\n" << sim.get() << endl;

    // autopilot setup.
    sim.put("bot autopilot power 100");
    sim.put("bot autopilot turn_rate 5");
    sim.put("bot autopilot speed 300");
    sim.put("drive brake 100");
    
    // get our current conditions
    while (true)
    {
      current = get_my_state(sim);
      target = get_target(sim);
      
      // Status report
      if ((counter++ % 25) == 0)
      {
        cout << runtime.interval_as_HMS()
          << "\n\t vct to target: " << target.location
          << "\n\t     target Vr: " << target.velocity
          << "\n\t      location: " << current.location
          << "\n\t       heading: " << current.attitude.z
          << "\n\t         speed: " << current.velocity.magnatude()
          << endl;
      };
      
      if (target.location.x == -1.0)
      {
        if (state != "searching")
        {
          // no current target to chase
          sim.put("bot autopilot off");
          sim.put("drive brake 100");
          sim.put("drive turn 0");
          state = "searching";
        };
      }
      else
      {
        if (state != "tracking")
        {
          // go get'm!
          sim.put("drive brake 0");
          sim.put("bot autopilot on");
          state = "tracking";
        };
        stringstream s;
        s << "bot autopilot heading " << target.location.y;
        sim.put(s.str());

      };
      const int t(1e6/20);
      std::chrono::microseconds st(t);
      this_thread::sleep_for(st);
    };
  }
  catch (...)
  {};
  
  
  
  sim.close();


  
};



























