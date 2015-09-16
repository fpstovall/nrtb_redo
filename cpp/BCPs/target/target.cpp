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
  config.read(argc, argv, "kamikaze.conf");
  
  // get the configuration data
  string server_addr(config.get<string>("server","127.0.0.1:64500"));
  
  tcp_socket sim;
  try
  {
    string sys, verb; 
    sim.connect(server_addr);
    // get bot ack.
    cout << "\n" << sim.getln() << endl;
    
    // get and publish our curent location.
    sim.put("bot lvar\r");
    // get bot ack.
    cout << "\n" << sim.getln() << endl;
    
      
    
  }
  catch (...)
  {};
  
  
  
  sim.close();


  
};



























