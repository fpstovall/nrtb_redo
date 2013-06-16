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

#include "tq_obj.h"

namespace nrtb_com
{

com_tq_obj_update::com_tq_obj_update()
{
  obj_uid = 0;
  event.clear();
  inbound_com.clear();
  outbound_com.clear();
};

com_tq_obj_update::com_tq_obj_update(nrtb_msg::sim_obj_tq_data* ext)
{
  set_from_message(ext);
};

void com_tq_obj_update::set_from_message(nrtb_msg::sim_obj_tq_data* ext)
{
  obj_uid = ext->obj_uid();
  location.set_from_message(ext->mutable_location());
  velocity.set_from_message(ext->mutable_velocity());
  // load the events
  int l = ext->event_size();
  for (int i=0; i<l; i++)
  {
	event.push_back(ext->event(i));
  };
  // load the inbound messages
  l = ext->inbound_comm_size();
  for (int i=0; i<l; i++)
  {
	inbound_com.push_back(ext->inbound_comm(i));
  };
  // load the outbound messages
  l = ext->outbound_comm_size();
  for (int i=0; i<l; i++)
  {
	outbound_com.push_back(ext->outbound_comm(i));
  };
};

void com_tq_obj_update::load_message(nrtb_msg::sim_obj_tq_data* ext)
{
  ext->set_obj_uid(obj_uid);
  location.load_message(ext->mutable_location());
  velocity.load_message(ext->mutable_velocity());
  // load the events
  typedef nrtb::strlist::iterator it;
  it c = event.begin();
  it e = event.end();
  while (c != e)
  {
	ext->add_event(*c);
	c++;
  };
  // load inbound com messages
  c = inbound_com.begin();
  e = inbound_com.end();
  while (c != e)
  {
	ext->add_inbound_comm(*c);
	c++;
  };
  // load outbound com messages
  c = outbound_com.begin();
  e = outbound_com.end();
  while (c != e)
  {
	ext->add_outbound_comm(*c);
	c++;
  };
};

bool com_tq_obj_update::operator ==(com_tq_obj_update& a)
{
  bool returnme = true;
  returnme = (a.obj_uid = obj_uid) && returnme;
  returnme = (a.location == location) && returnme;
  returnme = (a.velocity == velocity) && returnme;
  // check the events
  returnme = (a.event.size() == event.size()) && returnme;
  if (returnme)
  {
	int l = event.size();
	for (int i=0; i<l; i++)
	{
	  returnme = (a.event[i] == event[i]) && returnme;
	};
  };
  // check the inbound messages
  returnme = (a.inbound_com.size() == inbound_com.size()) && returnme;
  if (returnme)
  {
	int l = inbound_com.size();
	for (int i=0; i<l; i++)
	{
	  returnme = (a.inbound_com[i] == inbound_com[i]) && returnme;
	};
  };
  // check out outbound messages
  returnme = (a.outbound_com.size() == outbound_com.size()) && returnme;
  if (returnme)
  {
	int l = outbound_com.size();
	for (int i=0; i<l; i++)
	{
	  returnme = (a.outbound_com[i] == outbound_com[i]) && returnme;
	};
  };
  return returnme;
};
  
} // namespace nrtb::com

