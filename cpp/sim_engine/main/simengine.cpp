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

#include <common.h>
#include <logger.h>
#include <confreader.h>
#include <common_log.h>
#include <sim_core.h>
#include <bcp_server.h>

using namespace nrtb;
using namespace std;

int main(int argc, char * argv[])
{
  // load the global configuration
  conf_reader config;
  config.read(argc, argv, "simengine.conf");
  
  // start the system logger
  
  // create our recorder
  auto g_log(common_log::get_reference()("main()"));
  
  // Report our startup and configuration.
  g_log.info("Start up");
  g_log.info("Configuration Follows");
  for (auto i : config)
  {
    g_log.info(i.first+" = "+i.second);
  };
  g_log.info("Configuration list complete");
  
  // Any modules called from here should be passed the 
  // g_log_queue and config by reference. 
  
  // start the sim_core.
  float quanta = config.get<float>("quanta",1.0/50.0); 
  sim_core world(quanta);
  world.start_sim();
  
  // start the bcp_server
  bcp_listener bcps(world);
  bcps.start();
  
  
  chrono::seconds rt(1);
  this_thread::sleep_for(rt);
  

  bcps.stop();
  world.stop_sim(); 
  // say goodbye
  g_log.info("Shut down");
};



























