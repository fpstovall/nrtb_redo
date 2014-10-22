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
#include <ipc_channel.h>
#include <common_log.h>
#include <sim_core.h>
#include <bcp_server.h>

using namespace nrtb;
using namespace std;

void output_writer()
{
  ofstream output("simulation.out");
  try
  {
    // get the results from the sim_core.
    auto & ipc = global_ipc_channel_manager::get_reference();
    ipc_queue & soq = ipc.get("sim_output");
    gp_sim_message_adapter sim_out(soq);
    while (true)
    {
      gp_sim_message_p raw = sim_out.pop();
      sim_core::report & rep = raw->data<sim_core::report>();
      output << "quanta\t" << rep.quanta 
        << "\t" << rep.duration
        << "\t" << rep.wallclock 
        << endl;
      for(auto o : rep.objects)
        output << "obj\t" << o.second->as_str() << endl;
    };
  }
  catch (...)
  {};
  output.close();
  cout << "output writer closed." << endl;
};

int main(int argc, char * argv[])
{
  // load the global configuration
  conf_reader config;
  config.read(argc, argv, "simengine.conf");
  
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
  
  // start the sim output writer.
  std::thread writer(output_writer);
  
  // start the sim_core.
  float quanta = config.get<float>("quanta",1.0/50.0); 
  sim_core world(quanta);
  world.start_sim();
  
  // start the bcp_server
  bcp_listener bcps(world);
  bcps.start();
  
  int run_time = config.get("run_time",1);
  if (run_time > 0)
  {
    // programmed run time.
    chrono::seconds rt(run_time);
    this_thread::sleep_for(rt);
  }
  else
  {
    // run until stop is requested.
    cout << "\nPress enter to end.\n" << endl;
    char t = ' ';
    cin.get();
    cout << "shutting down" << endl;
  };
  // shut it all down.
  bcps.stop();
  world.stop_sim();
  // wait for output queue to drain.
  chrono::milliseconds pause(100);
  auto & soq = global_ipc_channel_manager::get_reference().get("sim_output");  
  while (soq.size()) this_thread::sleep_for(pause);
  // close out the writer.
  soq.shutdown();
  writer.join();
  // say goodbye
  g_log.info("Shut down");
};



























