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
  int t_time(config.get<int>("t_ms",440));
  string a_set(config.get<string>("a_set","100"));
  string b_set(config.get<string>("b_set","100"));
  string t_set(config.get<string>("t_set","100"));
  
  tcp_socket sim;
  try
  {
    string sys, verb; 
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
    stringstream response(sim.getln());
    triplet location;
    triplet velocity;
    response >> sys >> verb 
      >> location >> velocity;
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
    response.str(sim.getln());
    response >> sys >> verb 
      >> location >> velocity;
    float dec = start - velocity.x;
    cout << "\nDecelleration:\n\t"
      << dec << "(m/s) / " <<  elapsed
      << "(s) = " << dec/elapsed 
      << " m/s^2" << endl;
      
    // Turn test.
    // -- make sure we're stopped.  
    sim.put("bot lvar\r");
    response.str(sim.getln());      
    triplet attitude;
    triplet rotation;
    response >> sys >> verb 
      >> location >> velocity >> attitude >> rotation;
    if ((attitude.magnatude() != 0.0) 
      or (rotation.magnatude() != 0))
    {
      cout << "Bot already turning; test bypassed." << endl;
      base_exception e;
      throw e;
    };
    // conduct the test if we are okay.
    cout << "\nTurn Rate:" << endl;
    sim.put("drive turn "+t_set+"\r");
    hirez_timer turn_time;
    chrono::milliseconds tt(t_time);
    this_thread::sleep_for(tt);
    sim.put("bot lvar\rdrive turn 0\r");
    elapsed = turn_time.stop();
    response.str(sim.getln());
    response >> sys >> verb
      >> location >> velocity >> attitude >> rotation;
    float ad = (attitude.z/3.14159) * 180;
    float rd = (rotation.z/3.14159) * 180;
    cout << "\tSet Rate: " << rd << " d/s\n"
      << "\tMeasured: "<< (ad / elapsed) << " d/s" 
      << endl;
    
    // autopilot test
    sim.put("drive power 0\r");
    sim.put("drive brake 100\r");
    bool stopped {false};
    while (!stopped) 
    {
      this_thread::sleep_for(at);
      sim.put("bot lvar\r");
      response.str(sim.getln());
      response >> sys >> verb
        >> location >> velocity >> attitude >> rotation;
      velocity.z = 0;
      stopped = (velocity.magnatude() == 0.0 and rotation.z == 0.0);
    };
    cout << "\nReady for autopilot test." << endl;
    sim.put("bot autopilot status\r");
    cout << sim.getln() << endl;
    sim.put("bot autopilot heading 3.14\r");
    sim.put("bot autopilot power 100\r");
    sim.put("bot autopilot speed 50\r");
    sim.put("bot autopilot turn_rate 10\r");
    sim.put("bot autopilot status\r");
    cout << sim.getln() << endl;
    sim.put("drive brake 0\rbot autopilot on\rbot autopilot status\r");
    cout << sim.getln() << endl;
    bool done {false};
    while (!done)
    {
      this_thread::sleep_for(at);
      sim.put("bot lvar\r");
      response.str(sim.getln());
      response >> sys >> verb
        >> location >> velocity >> attitude >> rotation;
      velocity.z = 0;
      float heading = attitude.z;
      float vt = fabs(velocity.magnatude() - 50.0);
      float ht = fabs(heading - 3.14);
      done = (vt < 0.5) and (ht < 0.1);
      cout << vt << " : " << ht << " (" << heading << ")" << endl;
    }
    cout << "\nOn Course 1\n" << endl;
    sim.put("bot autopilot heading 6.0\rbot autopilot status\r");
    cout << sim.getln() << endl;
    done = false;
    while (!done)
    {
      this_thread::sleep_for(at);
      sim.put("bot lvar\r");
      response.str(sim.getln());
      response >> sys >> verb
        >> location >> velocity >> attitude >> rotation;
      velocity.z = 0;
      float heading = attitude.z;
      float vt = fabs(velocity.magnatude() - 50.0);
      float ht = fabs(heading - 6.0);
      done = (vt < 0.5) and (ht < 0.1);
      cout << vt << " : " << ht << " (" << heading << ")" << endl;
    }
    cout << "\nOn course 2\n" << endl;
    sim.put("bot autopilot heading 1.30\rbot autopilot speed 30\rbot autopilot status\r");
    cout << sim.getln() << endl;
    done = false;
    while (!done)
    {
      this_thread::sleep_for(at);
      sim.put("bot lvar\r");
      response.str(sim.getln());
      response >> sys >> verb
        >> location >> velocity >> attitude >> rotation;
      velocity.z = 0;
      float heading = attitude.z;
      float vt = fabs(velocity.magnatude() - 30.0);
      float ht = fabs(heading - 1.3);
      done = (vt < 0.5) and (ht < 0.1);
      cout << vt << " : " << ht << " (" << heading << ")" << endl;
    }
    cout << "\nOn course 3\n" << endl;
      
    cout << "\nRun complete" << endl;
    
  }
  catch (...)
  {};
  
  
  
  sim.close();


  
};



























