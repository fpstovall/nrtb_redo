/***********************************************
 This file is part of the NRTB project (https://*launchpad.net/nrtb).
 
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


#include "sim_db_channel.h"
#include <confreader.h>

using string;

namespace nrtb
{

void sim_db_channel::return_to_base_state()
{
  // shutdown the work threads, if any.
  if (input_manager) 
  {
	input_manager->stop();
	input_manager.reset();
  };
  if (output_manager) 
  {
	output_manager->stop();
	output_manager.reset();
  };
  // release the buffers
  out_queue.reset();
  in_queue.reset();
  // close and deallocate the transceiver
  link.reset();   
};

sim_db_channel::sim_db_channel()
{
  // get the global_conf_reader
  global_conf_reader & conf = global_conf_reader::get_instance();
  // get the values for our run.
  in_queue_limit = conf.get<int>("sim_db_channel.in_buff", 50);
  out_queue_limit = conf.get<int>("sim_db_channel.out_buff", 50);
};

sim_db_channel::~sim_db_channel()
{
  try { return_to_base_state(); }
  catch (...) {};
};

void sim_db_channel::establish_link(std::string address)
{
  // determine which address we should use.
  if (address == "na")
  {
	// get the global_conf_reader
	global_conf_reader & conf = global_conf_reader::get_instance();
	address = conf.get<std::string>("sim_db_channel.remote_addr",
		"127.0.0.1:63456");
  };
  // See if we need to add the port to the address.
  if (address.find(":") == std::string::npos)
  {
	address = address.append(":63456");
  };
  // okay.. at this point we should have the valid
  // address to start the transceiver with.
  string breadcrumbs = "init ";
  try
  {
	// TODO: instanciate link
	breadcrumbs.append("link ");
	// TODO: instanciate in_queue
	breadcrumbs.append("in_queue ");
	// TODO: instanciate out_queue
	breadcrumbs.append("out_queue");
	// TODO: instanciate the output_manager
	breadcrumbs.append("output_manager ");
	// TODO: Start output_manager
	breadcrumbs.append("started ");
	// TODO: instanciate the input_manager
	breadcrumbs.append("input_manager ");
	// TODO: Start input_manager
	breadcrumbs.append("started ");
  }
  catch (...)
  {
	// roll back the startup attempt
	return_to_base_state();
	// whine and complain.
	cant_connect_exception e;
	e.store(breadcrumbs);
	throw e;
  };
};

  

} // namespace nrtb




























