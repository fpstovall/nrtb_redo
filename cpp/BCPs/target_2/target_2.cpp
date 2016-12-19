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

const int run=1;
const int orient=2;
const int turning=3;
const int tofar=4;
const int returning=5;

std::string state_map(int state)
{
  string returnme = "WRONG";
  switch (state)
  {
    case run: { returnme="cruse"; break; }
    case orient: { returnme="orientation"; break;}
    case turning: { returnme="turning"; break; }
    case tofar: { returnme="zone exception"; break; }
    case returning: { returnme="seeking zone"; break; }
  };
  return returnme;
};

int main(int argc, char * argv[])
{
  hirez_timer returntime();
  tcp_socket sim;
  // load the global configuration
  conf_reader config;
  config.read(argc, argv, "target_2.conf");
  
  // display config choices.
  cout << endl;
  for (auto v: config) cout << v.first << "=" << v.second << endl;
  
  // get the configuration data
  string server_addr(config.get<string>("server","127.0.0.1:64500"));
  triplet seek(config.get<triplet>("seek",triplet(0,0,-1)));
  float zone(config.get<float>("zone",1000));
  float turnslop(config.get<float>("turnslop",0.6));
  float speedlimit(config.get<float>("zonelimit",150));
  
  try
  {
    string sys, verb;
    triplet center;
    lvar start;
    lvar current;
    bool power{false};
    hirez_timer runtime;

    // connect to the simulation.
    sim.connect(server_addr);
    
    // get bot ack.
    cout << "\n" << gsub(sim.getln("\r",0,1),"\r","") << endl;
    
    // get and record initial conditions
    sim.put("bot lvar\r");
    start.load(gsub(sim.getln("\r",0,1),"\r",""));
    current = start;
    (seek.z < 0) ? center = start.location : center = seek;
    
    // report starting status
    cout << "landed at " << start.location << endl
      << "zone center : " << center << endl
      << "  zone size : " << zone << endl
      << "      range : " << current.location.range(center) 
      << "  turn slop : " << turnslop
      << endl;
      
    sim.put("drive power 25\r");
    sim.put("drive brake 0\r");
    sim.put("bot autol\r");

    int state(run); 
    float wayhome(0.0);
    float tohome;
    long long unsigned counter(0);
    
    // working loop
    while (true) // literally loop till we die....
    {
      // get current conditions
      current.load(gsub(sim.getln("\r",0,1),"\r",""));
            
      if ((++counter % 50) == 0)
      {
        cout << runtime.interval_as_HMS()
          << "\n\t           State: " << state_map(state) 
          << "\n\t vct from center: " << (current.location-center).to_polar()
          << "\n\t    ground speed: " << current.velocity.magnatude()
          << "\n\t vehicle heading: " << current.attitude.z 
          << "\n\t   actual motion: " << 
            [&]{triplet t=current.velocity; t.z=0.0; return t.to_polar().y;}()
          << "\n\t calc'd way home: " << wayhome
          << endl;
        // ping to keep our connection alive.. 
        sim.put("bot ping\r");
        sim.getln("\r",0,1); // throw away the response.
      };
      
      if (wayhome < 0.0) 
      {
        sim.close();
        exit(1);
      };
      
      // check conditions
      switch (state)
      {
        case run : 
        {
          float v = current.velocity.magnatude();
          if (current.location.range(center) > zone) 
          {
            state = tofar;
          }
          else if ((v > speedlimit) and (power))
          {
            sim.put("drive power 0\r");
            power = false;
          }
          else if ((v < (speedlimit*0.95)) and (!power))
          {
            sim.put("drive power 25\r");
            power = true;
          }
          break;
        }
        case tofar :
        {
          sim.put("drive power 0\r");
          power = false;
          state = orient;
          break;
        }
        case orient : 
        {
          wayhome = (center - current.location).to_polar().y;
          if (wayhome<0.0) wayhome += 6.283185;
          // start turn
          sim.put("drive turn 10\r");
          // set state
          state = turning;
          break;
        }
        case turning : 
        {
          float diff = fabs(current.attitude.z - wayhome);
          // check to see if we have the heading yet.
          if (diff < turnslop)
          {
            // -- stop turn
            sim.put("drive turn 0\r");
            // -- start drive
            sim.put("drive power 25\r");
            power = true;
            // -- set status to run
            state = returning;
            tohome = current.location.range(center);
          }
          break;
        }
        case returning:
        {
          float dist = current.location.range(center);
          if (dist < zone)
          {
            state = run;
          }
          else if (dist > tohome)
          {
            // missed by that much.. stop and retry.
            state = tofar;
          };
          // ensure we continue to close;
          tohome = dist;
          break;
        }
      };
    };
  }
  catch (tcp_socket::not_open_exception & e)
  {
    cout << "Connection closed.\n" << endl;
  }
  catch (tcp_socket::general_exception &e)
  {
    cout << "General socket error" << endl;
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



























