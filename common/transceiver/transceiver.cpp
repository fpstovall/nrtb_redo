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
 
#include "transceiver.h"
#include <confreader.h>
#include <serializer.h>
#include <string>
#include <sstream>

using namespace nrtb;

serializer tscvr_sequence;

template <class out, class in>
transceiver::transceiver(transceiver<out,in>::sockp socket)
{
  // get the configuration parameters.
  global_conf_reader & config = global_conf_reader::get_instance();
  send_time_limit = config.get<unsigned int>("transceiver.send_timeout",2);
  attempt_recovery = config.get<bool>("transceiver.allow_recovery",true);
  error_run_limit = 
	config.get<unsigned int>("transceiver.max_consecutive_errors",10);
  sent_messages.resize(config.get<int>("transceiver.history_size",50));
  // set up logging
  std::stringstream s;
  s << logname << "_" << tscvr_sequence();
  log = Poco::Logger::get(s.str());
  // set up the socket.
  sock(socket);
  // annouce ourselves...
  log->trace("Instanciated."); 
  s.str("");
  s << "history_size=" << sent_messages.size()
    << ", send_timeout=" << send_time_limit
    << ", attempt_recovery=" << attempt_recovery
    << ", error_run_limit=" << error_run_limit
    << ", remote_address=" << sock->get_remote_address()
    << ", local_address=" << sock->get_local_address();
  log->trace(s.str());
};

template <class out, class in>
transceiver::~transceiver()
{
	log->trace("In ~transciever");
	// shutdown the socket.
	sock->close();
	// discard the sent messages list.
	sent_messages.clear();
	log->trace("shutdown complete.");
};

template <class out, class in>
inp transceiver::get()
{
  inp returnme(new in);
  string input = sock->getln();
  returnme->ParseFromString(input);
  /// message sequence checks go here
  if (last_inbound == 0)
  {
	last_inbound = returnme->msg_uid();
  }
  else
  {
	last_inbound++;
	int temp = returnme->msg_uid();
	if (temp != last_inbound)
	{ 
	  inbound_seq_error e();
	  stringstream message;
	  message << "Expected " << last_inbound
		<< " received "  << temp;
	  e.store(message.str());
	  throw e;
	};
  };
  return returnme;
};

template <class out, class in>
void transceiver::send(outp sendme)
{
  sendme->set_msg_uid(out_msg_num());
  stringstream output;
  output << sendme->SerializeAsString() << "\r";
  sock->put(output.str());
  sent_messages.push_back(out);
};

template <class out, class in>
void transceiver::nak_invalid_context(const unsigned long int msg_number)
{
  general_exception e;
  e.store("transceiver::nak_invalid_context not implemented.");
  throw e;
};

template <class out, class in>
void transceiver::nak_validation_error(const unsigned long int msg_number)
{
  general_exception e;
  e.store("transceiver::nak_validation_error not implemented.");
  throw e;
};

template <class out, class in>
void transceiver::handle_inbound_nak()
{
  general_exception e;
  e.store("transceiver::handle_inbound_nak not implemented.");
  throw e;  
};

template <class out, class in>
void transceiver::handle_outbound_nak()
{
  general_exception e;
  e.store("transceiver::handle_outbound_nak not implemented.");
  throw e;    
};


