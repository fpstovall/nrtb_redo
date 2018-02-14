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
#include <ipc_channel.h>
#include <common_log.h>
#include <sim_core.h>
#include <bcp_server.h>
#include <mongo/client/dbclient.h>
#include <mongo/bson/bson.h>
#include <boost/lexical_cast.hpp>
//#include <future>
#include <signal.h>

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
void output_writer(string id, string host, bool write_zeros=true,
                   bool write_bots=true)
{
  try
  {
    mongo::DBClientConnection db;
    // get the results from the sim_core.
    auto & ipc = global_ipc_channel_manager::get_reference();
    db.connect(host);
    ipc_queue & soq = ipc.get("sim_output");
    gp_sim_message_adapter sim_out(soq);
    while (true)
    {
      gp_sim_message_p raw = sim_out.pop();
      sim_core::report rep = raw->data<sim_core::report>();
      if (write_zeros or rep.objects.size())
      {
        // build basic data.
        mongo::BSONObjBuilder b;
        b.genOID();
        b << "sim_id" << id
          << "quanta" << unsigned(rep.quanta)
          << "obj_count" << unsigned(rep.objects.size())
          << "ticks" << unsigned(rep.duration)
          << "run_sec" << float(rep.wallclock);
        // buld objects array if enabled.
        if (write_bots)
        {
          mongo::BSONArrayBuilder obj_array;
          for (auto o : rep.objects)
          {
            obj_array << o;
            //o.second->as_str());
          };
          b.appendArray("objects",obj_array.arr());
        };
        // save the quanta to the database.
        db.insert("nrtb.quanta",b.obj());
      };
      // is it time for a checkpoint update?
      if ((rep.quanta % 50) == 0)
      {
        // Update the checkpoint field.
        db.update("nrtb.sim_setup",
          BSON("sim_id" << id),
          BSON("$set" << BSON("last_checkpoint" << unsigned(rep.quanta)))
        );
      };
      soq.task_done();
    };
  }
  catch (...) {}; 
  cout << "  Output writer closed." << endl;
};

string mk_run_id()
{
  std::time_t base = std::time(nullptr);
  std::tm now = *std::localtime(&base);
  std::stringstream s;
  unsigned long long year = now.tm_year*31557600l;
  unsigned long long day = now.tm_yday*24*3600;
  unsigned long long secs = (now.tm_hour*3600)+(now.tm_min*60)+now.tm_sec;
  s << year+day+secs;
  return s.str();
};

sig_atomic_t user_exit_request {0};

void sig_int_handler(int sig)
{
  user_exit_request = 1;
}

int main(int argc, char * argv[])
{
  // load the global configuration
  conf_reader config;
  config.read(argc, argv, "simengine.conf");

  string run_id = mk_run_id();
  
  // store the sim setup to database.
  mongo::client::initialize();
  mongo::DBClientConnection db;
  db.connect(config.get<string>("mongo","localhost"));
  mongo::BSONObjBuilder b;
  b.genOID();
  b.append("sim_id",run_id);
  for (auto i : config)
  {
    b.append(i.first,i.second);
  };
  b.append("hardware_threads",std::thread::hardware_concurrency());
  b.append("last_checkpoint",0);
  db.insert("nrtb.sim_setup",b.obj());
  
  // create our recorder
  auto g_log(common_log::get_reference()("main()"));
  
  // Report our startup and configuration.
  g_log.info("Start up");
  g_log.info("id: "+run_id);
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
  std::thread writer(output_writer, run_id,
                     config.get<string>("mongo","localhost"),
                     config.get<bool>("write_zeros",true),
                     config.get<bool>("write_bots",false));
  
  // start the sim_core.
  float quanta = config.get<float>("quanta",1.0/50.0); 
  sim_core & world = global_sim_core::get_reference();
  world.set_quanta(quanta);
  world.start_sim(std::thread::hardware_concurrency());
  
  // start the bcp_server
  bcp_listener bcps(world,
    config.get<std::string>("port","*:64500"),
    config.get<int>("pop_limit",100));
  bcps.start();

  int run_time = config.get("run_time",1);

  auto & soq = global_ipc_channel_manager::get_reference().get("sim_output");  

  // set sig_int handler;
  cout << "\nUse Control-C to manually exit" << endl;
  signal (SIGINT, sig_int_handler);

  // Run health check loop until failure or requested shutdown.
  bool done{false};
  long long int tock_limit = run_time * 10;
  long long int tocks = 0;
  std::chrono::milliseconds span (100);
  while (!done)
  {
    this_thread::sleep_for(span);
    tocks++;
    std::cout << "\rPopulation: " << world.size() 
      << " Out_Queue: " << soq.size()
      << " Time " << tocks/10.0 << "         " << std::flush;
    // user requested exit check
    if (user_exit_request)
    {
      done = true;
      cout << "\nUser requested exit" << endl;
    };
    // timeout check
    if ((run_time > 0) and (tocks >= tock_limit)) 
    {
      done = true;
      cout << "\nRun time limit reached" << endl;
    };
    // health checks
    bool hflag = world.running() and bcps.listening() and writer.joinable();
    if (!hflag) 
    { 
      done = true;
      cout << "\nThread health check failed: " << endl;
      if (!world.running()) cout << "\tSim_core down" << endl;
      if (!bcps.listening()) cout << "\tTCP listener down" << endl;
      if (!writer.joinable()) cout << "\tLog writer down" << endl;
    };
  };

  // shut it all down.
  cout << "shutting down" << endl;
  bcps.stop();
  cout << "  BCP Listener stopped." << endl;
  world.stop_sim();
  cout << "  simulation stopped." << endl;
  // wait for output queue to drain.
  soq.join();
  // close out the writer.
  soq.shutdown();
  if (writer.joinable()) writer.join();
  cout << "  Output writer stopped." << endl;
  // say goodbye
  g_log.info("Shutdown complete.");
  exit(0);
};



























