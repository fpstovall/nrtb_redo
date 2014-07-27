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

using namespace nrtb;
using namespace std;

int main(int argc, char * argv[])
{
  // load the global configuration
  conf_reader config;
  config.read(argc, argv, "simengine.conf");
  
  // start the system logger
  log_queue g_log_queue;
  log_file_writer g_log_writer(g_log_queue,
	config.get<string>("global_log_file","simengine.log"));
  // create our recorder
  log_recorder g_log("main",g_log_queue);
  
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
  
  
  // say goodbye
  g_log.info("Shut down");
};



























