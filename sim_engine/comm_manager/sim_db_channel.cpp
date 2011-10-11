cd /***********************************************
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
  // shutdown the queues and threads, if any.
  if (in_queue) in_queue->shutdown();
  if (out_queue) out_queue->shutdown();
  if (input_manager) 
  {
	input_manager->join();
	input_manager.reset();
  };
  if (output_manager) 
  {
	output_manager->join();
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
	in_queue.reset(new msg_buff(in_queue_limit));
	breadcrumbs.append("in_queue ");
	out_queue.reset(new msg_buff(out_queue_limit));
	breadcrumbs.append("out_queue ");
	tcp_socket_p sock(new tcp_socket);
	link.reset(new io_t(sock));
	breadcrumbs.append("link ");
	output_manager.reset(new out_processor(out_queue, link));
	breadcrumbs.append("output_manager ");
	input_manager.reset(new in_processor(in_queue, link));
	breadcrumbs.append("input_manager ");
	sock->connect(address);
	breadcrumbs.append("connected ");
	output_manager->start();
	breadcrumbs.append("out_started ");
	input_manager->start();
	breadcrumbs.append("in_started ");
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

void sim_db_channel::sim_start(sim_db_channel::sim_info data)
{
  // send the sim setup information
  msg_p msg(new nrtb_msg::sim_setup_data());
  msg->set_name(data.name);
  msg->set_model(data.model);
  msg->set_quanta_ms(data.quanta_ms);
  msg->set_max_quantas(data.max_quantas);
  msg->set_started_by(data.started_by);
  // assemble the new objects
  obj_setup_list::iterator c = data.items.begin();
  obj_setup_list::iterator e = data.items.end();
  while (c != e)
  {
	// get a new obj_setup_data.
	nrtb_msg::obj_setup_data * item;
	item = msg->add_item();
	// map the data..
	item->set_uid(c->uid);
	item->set_name(c->name);
	item->set_obj_type(c->obj_type);
	//TODO: More to do here.
	
  }
  // message is done, send it.
  out_queue->push(msg);
  
  
};

sim_db_channel::in_processor::in_processor(
  sim_db_channel::msg_buff_p buf, io_p io)
{

};

sim_db_channel::in_processor::run()
{

};

sim_db_channel::out_processor::out_processor(
  sim_db_channel::msg_buff_p buf, io_p io)
{

};

sim_db_channel::out_processor::run()
{

};


} // namespace nrtb




























