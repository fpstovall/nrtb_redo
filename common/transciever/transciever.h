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
 
#ifndef nrtb_transciever_h
#define nrtb_transciever_h

#include <string>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <boost/circular_buffer.hpp>

namespace nrtb
{
  template <class out_gpb, class in_gpb>
  class transciever
  {
	public:
	  typedef out_gpb outbound_type;
	  typedef in_gpb inbound_type;
	  transciever(Poco::Net::StreamSocket socket);
	  ~transciever();
	  inbound_type & get();
	  void send(const & outbound_type);
	protected:
	  Poco::Net::StreamSocket sock;
	  Poco::Net::SocketStream stream;
	  boost::circular_buffer<outbound_type> sent_messages;
	private:
  };

} // namespace nrtb
 
#endif //nrtb_transciever_h