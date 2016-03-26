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
#include <mongo/client/dbclient.h>
#include <mongo/bson/bson.h>
#include <boost/lexical_cast.hpp>

using namespace nrtb;
using namespace std;

/*****************************************************
 * output_writer drains the sim_core output queue and 
 * writes the output to a formatted text file. main()
 * launches this as an independent thread which is 
 * left to run until the output queue is closed.
 * 
 * As we move out of alpha, this needs to be moved
 * out of the main file and into it's own more complete
 * module.
 ****************************************************/
void output_writer(bool write_zeros=true)
{
  ofstream output("simulation.out");
  try
  {
    mongo::DBClientConnection db;
    db.connect("localhost");
    // get the results from the sim_core.
    auto & ipc = global_ipc_channel_manager::get_reference();
    ipc_queue & soq = ipc.get("sim_output");
    gp_sim_message_adapter sim_out(soq);
    while (true)
    {
      gp_sim_message_p raw = sim_out.pop();
      sim_core::report rep = raw->data<sim_core::report>();
      if (write_zeros or rep.objects.size())
      {
        mongo::BSONObjBuilder b;
        b.genOID();
        b.append("quanta",boost::lexical_cast<string>(rep.quanta));
        b.append("obj_count",boost::lexical_cast<string>(rep.objects.size()));
        b.append("ticks",boost::lexical_cast<string>(rep.duration));
        b.append("run_ticks",boost::lexical_cast<string>(rep.wallclock));
        for(auto o: rep.objects)
        {
          string on = "obj_"+boost::lexical_cast<string>(o.second->id);
          b.append(on,o.second->as_str());
        };
        auto storeme = b.obj();
        db.insert("nrtb.quanta",storeme);
      };
    };
  }
  catch (std::exception &e)
  {
    cerr << e.what() << endl;
  }
  catch (...)
  {
    cerr << "output_writer: Unspecified error caught" << endl;
  };
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
  std::stringstream s;
  s << std::thread::hardware_concurrency() 
    << " hardware threads supported.";
  g_log.info("Configuration Follows");
  g_log.info(s.str());
  for (auto i : config)
  {
    g_log.info(i.first+" = "+i.second);
  };
  g_log.info("Configuration list complete");
  
  // start the sim output writer.
  std::thread writer(output_writer,config.get<bool>("write_zeros",true));
  
  // start the sim_core.
  float quanta = config.get<float>("quanta",1.0/50.0); 
  sim_core & world = global_sim_core::get_reference();
  world.set_quanta(quanta);
  world.start_sim();
  
  // start the bcp_server
  bcp_listener bcps(world,
    config.get<std::string>("port","*:64500"),
    config.get<int>("pop_limit",100));
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



























