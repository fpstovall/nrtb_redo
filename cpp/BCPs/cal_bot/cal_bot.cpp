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
#include <triad.h>

using namespace nrtb;
using namespace std;

typedef triad<float> triplet;

int main(int argc, char * argv[])
{
  hirez_timer runtime();
  // load the global configuration
  conf_reader config;
  config.read(argc, argv, "cal_bot.conf");
  
  // get the configuration data
  string server_addr(config.get<string>("server","127.0.0.1:64500"));
  int a_time(config.get<int>("a_ms",1000));
  int b_time(config.get<int>("b_ms",500));
  string a_set(config.get<string>("a_set","100"));
  string b_set(config.get<string>("b_set","100"));
  
  tcp_socket sim;
  try
  {
    sim.connect(server_addr);
    // get bot ack.
    cout << "\n" << sim.getln() << endl;
    
    //Accelleration test.
    sim.put("drive brake 0\r");
    sim.put("drive power "+a_set+"\r");
    hirez_timer atest;
    chrono::milliseconds at(a_time);
    this_thread::sleep_for(at);
    sim.put("drive power 0\rbot lvar\r");
    float elapsed = atest.stop();
    stringstream response(gsub(sim.getln(),")(",") ("));
    triplet location;
    triplet velocity;
    response >> location >> velocity;
    cout << "Accelleration:\n\t"
      << velocity.x << "(m/s) / " <<  elapsed
      << "(s) = " << velocity.x/elapsed 
      << " m/s^2" << endl;
      
    // Braking test.
    float start = velocity.x;
    sim.put("drive brake "+b_set+"\r");
    hirez_timer brake_time;
    chrono::milliseconds bt(b_time);
    this_thread::sleep_for(bt);
    sim.put("bot lvar\r");
    elapsed = brake_time.stop();
    response.str(gsub(sim.getln(),")(",") ("));
    response >> location >> velocity;
    float dec = start - velocity.x;
    cout << "\nDecelleration:\n\t"
      << dec << "(m/s) / " <<  elapsed
      << "(s) = " << dec/elapsed 
      << " m/s^2" << endl;
    
      
    
    sim.close();
  }
  catch (...)
  {};
  
  
  


  
};



























