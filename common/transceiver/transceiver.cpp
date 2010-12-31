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
#include "../include/confreader.h"
#include <Poco/Mutex.h>
#include <Poco/ScopedLock.h>
#include <stringstream>

class serializer_type
{
public:
  unsigned int operator()
  {
	Poco::ScopedLock<Poco::Mutex>(lock);
	return ++counter;
  };
private:
  Poco::Mutex lock;
  unsigned int counter;
  serializer_type()
  {
	counter = 0;
  };
};

serializer_type serializer;  

using namespace nrtb;

template <class out, class in>
transceiver::transceiver(Poco::Net::StreamSocket socket)
{
  // get the configuration parameters.
  conf_reader & config = conf_reader::get_instance();
  send_time_limit = config.get<unsigned int>("transceiver.send_timeout",2);
  attempt_recovery = config.get<bool>("transceiver.allow_recovery",true);
  error_run_limit = 
	config.get<unsigned int>("transceiver.max_consecutive_errors",10);
  sent_messages.resize(config.get<int>("transceiver.history_size",50));
  // set up uid
  uid = serializer();
  // set up logging
  std::stringstream s << logname << "_" << uid ;
  log = Poco::Logger::get(s.str());
};

template <class out, class in>
transceiver::~transceiver()
{
  
};

template <class out, class in>
in transceiver::get()
{
  
};

template <class out, class in>
void transceiver::send(out & sendme)
{
  
};

template <class out, class in>
void transceiver::nak_invalid_context(const unsigned long int msg_number)
{
  
};

template <class out, class in>
void transceiver::nak_validation_error(const unsigned long int msg_number)
{
  
};

template <class out, class in>
void transceiver::handle_inbound_nak()
{
  
};

template <class out, class in>
void transceiver::handle_outbound_nak()
{
  
};


