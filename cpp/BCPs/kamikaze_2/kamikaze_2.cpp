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
#include <map>
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

class game_state
{
public:
  lvar me;
  lvar target;
  bool tracking {false};
  bool power {false};
  int turning {0}; // -1, 0, 1
  bool update(tcp_socket & s)
  {
    tracking = false;
    string msg = gsub(s.getln("\r",0,1),"\r","");
    stringstream tokens(msg);
    string sys, cmd;
    tokens >> sys >> cmd;
    if (sys == "bot" and cmd == "lvar")
    {
      got_lvar = true;
      me.load(msg);
    }
    else if (sys == "radar" and cmd == "contacts")
    {
      got_contact = true;
      int count {0};
      tokens >> count;
      if (count > 0)
      {
        tracking = true;
        string junk;
        tokens >> junk >> target.location >> target.velocity;
      };
    }
    else 
    {
      cout << "sim >> " << msg << endl;
    };
    bool returnme = got_lvar and got_contact;
    if (returnme)
    {
      got_lvar = got_contact = false;
    }
    return returnme;
  };
private:
  bool got_lvar {false};
  bool got_contact {false};
};

int main(int argc, char * argv[])
{
  hirez_timer runtime;
  // load the global configuration
  conf_reader config;
  config.read(argc, argv, "kamikaze_2.conf");
  cout << endl;
  
  // get the configuration data
  string server_addr(config.get<string>("server","127.0.0.1:64500"));
  
  // com socket
  tcp_socket sim;
  
  try
  {
    int counter {0};
	game_state world;
    // connect and get bot ack.
    sim.connect(server_addr);
    sim.put("bot autol\r");
    sim.put("bot autor\r");
    
    // get our current conditions
    while (true)
    {
      while (!world.update(sim)) {};
      
      // Status report
      if ((counter++ % 25) == 0)
      {
    	triplet temp = world.me.velocity;
    	temp.z = 0;
        cout << runtime.interval_as_HMS()
          << "\n\t vct to target: " << world.target.location
          << "\n\t     target Vr: " << world.target.velocity
          << "\n\t      location: " << world.me.location
          << "\n\t       heading: " << world.me.attitude.z << "\t" << (world.tracking ? "Tracking" : "Not Tracking")
          << "\n\t         speed: " << temp.magnatude() << "\tPower " << (world.power ? "On" : "Off")
          << endl;
      };
      
      if (!world.tracking)
      {
        // no current target to chase
    	if (world.power)
    	{
          sim.put("drive power 0\r");
          sim.put("drive brake 100\r");
          world.power = false;
    	};
    	if (world.turning != 0) 
    	{
    	  world.turning = 0; 
    	  sim.put("drive turn 0\r");
    	}
      }
      else
      {
        // go get'm!
    	if (!world.power)
    	{
          sim.put("drive brake 0\r");
          sim.put("drive power 100\r");
          world.power = true;
    	};
        // figure out our course changes. 
        float delta = (world.target.location.y - world.me.attitude.z);
        if (delta < -0.3 and world.turning != -1) 
          { world.turning = -1; sim.put("drive turn -10\r"); }
        else if (delta > -0.3 and world.turning != 1) 
          { world.turning = 1; sim.put("drive turn 10\r"); }
        else if (world.turning != 0) { world.turning = 0;  sim.put("drive turn 0\r"); };
      };
    };
    
  }
  catch (tcp_socket::not_open_exception & e)
  {
    cout << "Connction Closed" << endl;
  }
  catch (tcp_socket::general_exception & e)
  {
    cout << "Socket Error" << endl;
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
  try {sim.close();} catch (...) {};
};



























