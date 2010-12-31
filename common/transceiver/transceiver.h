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
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/Logger.h>
#include <Poco/Exception.h>
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
  template <class out, class in>
  class transceiver
  {
	public:
	  /// outbound messages will be of this type
	  typedef outbound_type out;
	  /// inbound messages will be of this type.
	  typedef inbound_type in;
	  /**************************************************************
	   * Creates the transceiver and associates it with a provided 
	   * socket. Once created this class assumes it uniquely owns the 
	   * socket and will close it upon distruction.
	   * ***********************************************************/
	  transceiver(Poco::Net::StreamSocket socket);
	  /**************************************************************
	   * Closes the socket and releases all mmemory associated with
	   * this class.
	   * ***********************************************************/
	  ~transceiver();
	  /**************************************************************
	   * gets the next message from the socket. If no messages are 
	   * ready, blocks util one arrives. 
	   * ***********************************************************/
	  in & get();
	  /**************************************************************
	   * Sends a message over the socket and adds it to the 
	   * sent_messages buffer in case it's needed for error recovery.
	   * ***********************************************************/
	  void send(out & sendme);
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
	  // parent of all transceiver exceptions
	  POCO_DECLARE_EXCEPTION(transceiver, general_exception, Poco::Exception)
	  // thrown if send fails due to timeout.
	  POCO_DECLARE_EXCEPTION(transceiver, send_timeout, general_exception)
	  // thrown in the case of a fault while in get()
	  POCO_DECLARE_EXCEPTION(transceiver, get_fault, general_exception)
	  // Thrown if the socket is closed due to an unrecoverable date error.
	  POCO_DECLARE_EXCEPTION(transceiver, unrecoverable_data_error, general_exception)
	  // Thrown if the socket is closed due to too many errors in a row
	  POCO_DECLARE_EXCEPTION(transceiver, consecutive_error_overrun, general_exception)
	protected:
	  unsigned in uid;
	  const std::string logname = "transceiver:";
	  unsigned int send_time_limit;
	  bool attempt_recovery;
	  unsigned int error_run_limit;
	  // pointer to this class's logger instance
	  Poco::Logger * log;
	  // The socket used for communcation.
	  Poco::Net::StreamSocket sock;
	  // the associated iostream (Do we need this?)
	  Poco::Net::SocketStream stream;
	  // buffer to hold previously sent messages; required for 
	  // error recovery.
	  boost::circular_buffer<out> sent_messages;
	  // fence post for recovery efforts, zero if none in play
	  unsigned long long nak_fence_post;
	  // These methods implment actual nak recovery.
	  void handle_inbound_nak();
	  void handle_outbound_nak();
  };

} // namespace nrtb
 
#endif //nrtb_transceiver_h//