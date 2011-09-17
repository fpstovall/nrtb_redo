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
 
#ifndef nrtb_transceiver_h
#define nrtb_transceiver_h

#include <string>
#include <sstream>
#include <common.h>
#include <base_socket.h>
#include <serializer.h>
#include <confreader.h>
#include <Poco/Logger.h>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>

namespace nrtb
{
  /*******************************************************************
   * transceiver is used to manage GBP (google protocol buffers)
   * traffic between NRTB components. This is the only form of 
   * IPC used by NRTB.
   * 
   * out is the channel wrapper for the outbound channel.
   * in is the channel wrapper for the inbound channel.
   * 
   * The nrtb::confreader singleton will be queried for the 
   * following parameters:
   * 
   * 	transceiver.history_size			(int)
   * 	transceiver.send_timeout			(int)
   * 	transceiver.allow_recovery			(bool)
   * 	transceiver.max_consecutive_errors	(int)
   * 
   * See https://blueprints.launchpad.net/nrtb/+spec/icp-spec for
   * specification this class implements.
   * ***************************************************************/
  template <class out, class in,
	class outp = boost::shared_ptr<out>,
	class inp = boost::shared_ptr<in> >
  class transceiver
  {
	public:
	  typedef inp in_ptr;
	  typedef outp out_ptr;
	  
	  union msg_num_t
	  {
		uint32_t number;
		unsigned char bytes[4];
	  };
	  
	  /*************************************************************
	   * Creates the transceiver and associates it with a provided 
	   * socket. Once created this class assumes it uniquely owns the 
	   * socket and will close it upon distruction.
	   * ***********************************************************/
	  transceiver(tcp_socket_p socket);
	  /*************************************************************
	   * Closes the socket and releases all mmemory associated with
	   * this class.
	   * ***********************************************************/
	  virtual ~transceiver();
	  /**************************************************************
	   * gets the next message from the socket. If no messages are 
	   * ready, blocks util one arrives. 
	   * ***********************************************************/
	  inp get();
	  /**************************************************************
	   * Sends a message over the socket and adds it to the 
	   * sent_messages buffer in case it's needed for error recovery.
	   * ***********************************************************/
	  void send(outp sendme);
	  /**************************************************************
	   * Called by the data consumer when an inbound message was 
	   * not valid in the current application context. msg_number
	   * is the sequence number of the offending message.
	   * ***********************************************************/
	  void nak_invalid_context(const unsigned long int msg_number);
	  /**************************************************************
	   * Called by the data consumer when an inbound message's 
	   * data failed validation checks. msg_number is the sequence
	   * number of the offending message.
	   * ***********************************************************/
	  void nak_validation_error(const unsigned long int msg_number);
	  /**************************************************************
	   * Exceptions which may be thrown for external resulution.
	   * ***********************************************************/
	  /// parent of all transceiver exceptions
	  class general_exception : public nrtb::base_exception {};
	  /// thrown if the a unexpected msg_uid is received
	  class inbound_seq_error : public general_exception {};
	  /// thrown the connection is unexpectedly lost
	  class connection_lost: public general_exception {};
	  /// thrown if too many sequencial errors occur.
	  class too_many_errors: public general_exception {};
	  
  protected:
	  unsigned int send_time_limit;
	  bool attempt_recovery;
	  unsigned int error_run_limit;
	  /// the name used for logging
	  std::string logname;
	  /// The socket used for communcation.
	  tcp_socket_p sock;
	  /// serializer used for message numbers
	  serializer out_msg_num;
	  /// last received message number 
	  unsigned long long  last_inbound;
	  /// buffer to hold previously sent messages; required for 
	  /// error recovery.
	  boost::circular_buffer<outp> sent_messages;
	  /// fence post for recovery efforts, zero if none in play
	  unsigned long long nak_fence_post;
	  /// These methods implment actual nak recovery.
	  void handle_inbound_nak();
	  void handle_outbound_nak();
  };
  

serializer tscvr_sequence(0);

template <class out, class in, class outp, class inp>
transceiver<out,in,outp,inp>::transceiver(tcp_socket_p socket)
{
  // get the configuration parameters.
  global_conf_reader & config = global_conf_reader::get_instance();
  send_time_limit = config.get<unsigned int>("transceiver.send_timeout",2);
  attempt_recovery = config.get<bool>("transceiver.allow_recovery",true);
  error_run_limit = 
	config.get<unsigned int>("transceiver.max_consecutive_errors",10);
  sent_messages.resize(config.get<int>("transceiver.history_size",50));
  last_inbound = 0;
  // set up logging
  std::stringstream s;
  s << "transceiver:_" << tscvr_sequence();
  logname = s.str();
  Poco::Logger & log = Poco::Logger::get(logname);
  // set up the socket.
  sock = socket;
  // annouce ourselves...
  log.information("Instanciated."); 
  s.str("");
  s << "history_size=" << sent_messages.size()
    << ", send_timeout=" << send_time_limit
    << ", attempt_recovery=" << attempt_recovery
    << ", error_run_limit=" << error_run_limit
    << ", remote_address=" << sock->get_remote_address()
    << ", local_address=" << sock->get_local_address();
  log.information(s.str());
};

template <class out, class in, class outp, class inp>
transceiver<out,in,outp,inp>::~transceiver()
{
  Poco::Logger & log = Poco::Logger::get(logname);
  log.information("In ~transciever");
  // shutdown and release  the socket.
  try 
  {
	if (sock)
	{
	  sock.reset(); 
	};
  } catch (...) {};
  // discard the sent messages list.
  sent_messages.clear();
  log.information("shutdown complete.");
};

template <class out, class in, class outp, class inp>
inp transceiver<out,in,outp,inp>::get()
{
  // get the message length first.
  std::string len_field = sock->get(4,10);
//std::cout << "len_field=" << http_chartohex(len_field) << std::endl;  
  msg_num_t msg_len;
  for (int i=0; i<4; i++)
  {
	msg_len.bytes[i] = len_field[i];
  };
//std::cout << ":len=" << msg_len.number << std::endl;
  // get the rest of the message.
  inp returnme(new in);
  std::string input = sock->get(msg_len.number);
//std::cout << ":received=" << http_chartohex(input) << std::endl;
  returnme->ParseFromString(input);
  // for the first messsge any number is
  // accepted.
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
	  inbound_seq_error e;
	  std::stringstream message;
	  message << "Expected " << last_inbound
		<< " received "  << temp;
	  e.store(message.str());
	  throw e;
	};
  };
  return returnme;
};

template <class out, class in, class outp, class inp>
void transceiver<out,in,outp,inp>::send(outp sendme)
{
  sendme->set_msg_uid(out_msg_num());
  std::string output;
  output = sendme->SerializeAsString();
  msg_num_t msg_len;
  msg_len.number = output.size();
//std::cout << "num:len" << msg_len.number << ":" << output.length() << //std::endl;
  std::string num_field = "    ";
  for (int i=0; i<4; i++)
  {
	num_field[i] = msg_len.bytes[i];
//std::cout << int(num_field[i]) << "," ;
  };
//std::cout << " = " << msg_len.number << std::endl;  
  output = num_field + output;
//std::cout << "out msg=" << http_chartohex(output) << std::endl;  
  sock->put(output);
  sent_messages.push_back(sendme);
};

template <class out, class in, class outp, class inp>
void transceiver<out,in,outp,inp>::nak_invalid_context(const unsigned long int msg_number)
{
  general_exception e;
  e.store("transceiver<outp,inp>::nak_invalid_context not implemented.");
  throw e;
};

template <class out, class in, class outp, class inp>
void transceiver<out,in,outp,inp>::nak_validation_error(const unsigned long int msg_number)
{
  general_exception e;
  e.store("transceiver<outp,inp>::nak_validation_error not implemented.");
  throw e;
};

template <class out, class in, class outp, class inp>
void transceiver<out,in,outp,inp>::handle_inbound_nak()
{
  general_exception e;
  e.store("transceiver<outp,inp>::handle_inbound_nak not implemented.");
  throw e;  
};

template <class out, class in, class outp, class inp>
void transceiver<out,in,outp,inp>::handle_outbound_nak()
{
  general_exception e;
  e.store("transceiver<outp,inp>::handle_outbound_nak not implemented.");
  throw e;    
};

} // namespace nrtb
 
#endif //nrtb_transceiver_h//