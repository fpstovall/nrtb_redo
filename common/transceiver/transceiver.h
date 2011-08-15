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
#include <base_socket.h>
#include <serializer.h>
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
  
  /****************************************
   * This class is used ot manage all interprocess
   * communications between programs in the NRTB
   * system.
   * 
   * inp and outp are expected to be types 
   * of the form boost::shared_ptr<GPB> message,
   * normally the channel wrappers for the 
   * link to be managed. 
   ***************************************/
  template <class out, class in,
	class outp = boost::shared_ptr<out>,
	class inp = boost::shared_ptr<in> >
  class transceiver
  {
	public:
	  
	  /*************************************************************
	   * Creates the transceiver and associates it with a provided 
	   * socket. Once created this class assumes it uniquely owns the 
	   * socket and will close it upon distruction.
	   * ***********************************************************/
	  transceiver(tcp_socketp socket);
	  /*************************************************************
	   * Closes the socket and releases all mmemory associated with
	   * this class.
	   * ***********************************************************/
	  ~transceiver();
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
	  /// pointer to this class's logger instance
	  Poco::Logger * log;
	  /// The socket used for communcation.
	  tcp_socketp sock;
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

} // namespace nrtb
 
#endif //nrtb_transceiver_h//