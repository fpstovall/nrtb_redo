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

namespace nrtb
{

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
  // shutdown the work threads, if any.
  if (input_manager) input_manager.reset();
  if (output_manager) output_manager.reset();
  // close and deallocate the transceiver
  if (link) link.reset(); 
  // release the buffers
  if (out_queue) out_queue.reset();
  if (in_queue) in_queue.reset();
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
  if (address.find(":") == string::npos)
  {
	address = address.append(":63456");
  };
  // okay.. at this point we should have the valid
  // address to start the transceiver with.
  // TODO: instanciate link
  // TODO: instanciate in_queue
  // TODO: instanciate out_queue
  // TODO: instanciate the output_manager
  // TODO: Start output_manager
  // TODO: instanciate the input_manager
  // TODO: Start input_manager
}


} // namespace nrtb




























