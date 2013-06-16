/***********************************************
 This file is part of the NRTB project (https://launchpad.net/nrtb).
 
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
 
#ifndef ack_nak_header
#define ack_nak_header

#include <ack_nak.pb.h>
#include <stdint.h>

namespace nrtb_com {

class com_ack
{
public:
  
  uint32_t msg_uid;
  
  com_ack();
  com_ack(nrtb_msg::message_ack * ext);
  virtual ~com_ack() {};

  void set_from_message(nrtb_msg::message_ack * ext);
  void load_message(nrtb_msg::message_ack * ext);  
  
  bool operator ==(com_ack & a);
};

class com_nak
{
public:
  enum codes
  {
	none,
	bad_message_length, 
	bad_message_parse,
	overflow,
	invalid_message,
	sequence_error
  };
  
  codes reason;
  uint32_t msg_uid;
  uint32_t last_good_msg_uid;
  
  com_nak();
  com_nak(nrtb_msg::message_nak * ext);
  virtual ~com_nak() {};
  
  void set_from_message(nrtb_msg::message_nak * ext);
  void load_message(nrtb_msg::message_nak * ext);
  
  bool operator ==(com_nak & a);
};

} // namespace nrtb::com

#endif // ack_nak_header
