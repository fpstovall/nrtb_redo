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

#include "tq_update.h"
#include <boost/shared_ptr.hpp>


namespace nrtb_com
{
com_tq_update::com_tq_update()
{
  quanta_id = 0;
  sim_uid = 0;
};

com_tq_update::com_tq_update(nrtb_msg::tq_data* ext)
{
  set_from_message(ext);
};

void com_tq_update::set_from_message(nrtb_msg::tq_data* ext)
{
  quanta_id = ext->quanta_id();
  sim_uid = ext->sim_uid();
  // load the updates
  int l = ext->obj_data_size();
  for (int i=0; i<l; i++)
  {
	com_tq_obj_update update(ext->mutable_obj_data(i));
	obj_data.push_back(update);
  };
  // load the new objects
  l = ext->new_objs_size();
  for (int i=0; i<l; i++)
  {
	com_obj_setup obj(ext->mutable_new_objs(i));
	new_objs.push_back(obj);
  };
};

void com_tq_update::load_message(nrtb_msg::tq_data* ext)
{
  ext->set_quanta_id(quanta_id);
  ext->set_sim_uid(sim_uid);
  {
	// load the updates
	up_list::iterator c = obj_data.begin();
	up_list::iterator e = obj_data.end();
	while (c != e)
	{
	  c->load_message(ext->add_obj_data());
	  c++;
	};
  }
  {
	// load the new objs
	ob_list::iterator c = new_objs.begin();
	ob_list::iterator e = new_objs.end();
	while (c != e)
	{
	  c->load_message(ext->add_new_objs());
	  c++;
	};
  }
};

bool com_tq_update::operator==(com_tq_update& a)
{
  bool returnme = true;
  returnme = (quanta_id == a.quanta_id) && returnme;
  returnme = (sim_uid == a.sim_uid) && returnme;
  // check the updated objects list
  returnme = (obj_data.size() == a.obj_data.size()) && returnme;
  if (returnme)
  {
	int l = obj_data.size();
	for (int i=1; i<l; i++)
	{
	  returnme = (obj_data[i] == a.obj_data[i]) && returnme;
	};
  };
  // check the new objects list
  returnme = (new_objs.size() == a.new_objs.size()) && returnme;
  if (returnme)
  {
	int l = new_objs.size();
	for (int i=0; i<l; i++)
	{
	  returnme = (new_objs[i] == a.new_objs[i]) && returnme;
	};
  };
};

} // namespace nrtb::com

