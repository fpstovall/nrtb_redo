/***********************************************
 This file is part of the NRTB project (https://github.com/fpstovall/nrtb_beta).
 
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

#include "ack_nak.h"

#include <iostream>

namespace nrtb_com
{

com_ack::com_ack()
{
  msg_uid = 0;
};

com_ack::com_ack(nrtb_msg::message_ack* ext)
{
  set_from_message(ext);
};

void com_ack::set_from_message(nrtb_msg::message_ack* ext)
{
  msg_uid = ext->msg_uid();
};

void com_ack::load_message(nrtb_msg::message_ack* ext)
{
  ext->set_msg_uid(msg_uid);
};

bool com_ack::operator==(com_ack& a)
{
  return msg_uid == a.msg_uid;
};

com_nak::com_nak()
{
  reason = none;
  msg_uid = 0;
  last_good_msg_uid = 0;
};

com_nak::com_nak(nrtb_msg::message_nak* ext)
{
  set_from_message(ext);
};

void com_nak::set_from_message(nrtb_msg::message_nak* ext)
{
  reason = codes(ext->reason());
  if (ext->has_msg_uid())
	msg_uid = ext->msg_uid();
  else msg_uid = 0;
  if (ext->has_last_good_msg_uid())
	last_good_msg_uid = ext->last_good_msg_uid();
  else last_good_msg_uid = 0;
};

void com_nak::load_message(nrtb_msg::message_nak* ext)
{
  ext->set_reason(reason);
  if (msg_uid) ext->set_msg_uid(msg_uid);
	else ext->clear_msg_uid();
  if (last_good_msg_uid)
	ext->set_last_good_msg_uid(last_good_msg_uid);
  else ext->clear_last_good_msg_uid();
};

bool com_nak::operator==(com_nak& a)
{
  bool returnme = true;
  returnme = (reason == a.reason) && returnme;
  returnme = (msg_uid == a.msg_uid) && returnme;
  returnme = (last_good_msg_uid == a.last_good_msg_uid) && returnme;
  return returnme;
};

} // namespace nrtb::com

