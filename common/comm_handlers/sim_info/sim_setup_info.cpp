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

#include "sim_setup_info.h"

#include <iostream>

namespace nrtb_com
{

com_sim_setup::com_sim_setup()
{
  name = "uninitialized";
  model = name;
  quanta_ms = 0;
  max_quantas = 0;
  started_by = name;
  items.clear();
};

com_sim_setup::com_sim_setup(nrtb_msg::sim_setup_data* ext)
{
  set_from_message(ext);
};

void com_sim_setup::set_from_message(nrtb_msg::sim_setup_data* ext)
{
  // load the singular variables.
  name = ext->name();
  model = ext->model();
  quanta_ms = ext->quanta_ms();
  max_quantas = ext->max_quantas();
  started_by = ext->started_by();
  // process the listed objects
  int count = ext->item_size();
  for (int i=0; i<count; i++)
  {
	obj_p t(new com_obj_setup(ext->mutable_item(i)));
	items.push_back(t);
  };
};

void com_sim_setup::load_message(nrtb_msg::sim_setup_data* ext)
{
  // set the singular variables
  ext->set_name(name);
  ext->set_model(model);
  ext->set_quanta_ms(quanta_ms);
  ext->set_max_quantas(max_quantas);
  ext->set_started_by(started_by);
  // process the items list
  ext->clear_item();
  obj_list::iterator c = items.begin();
  obj_list::iterator e = items.end();
  while (c != e)
  {
	(*c)->load_message(ext->add_item());
	c++;
  };
};

bool com_sim_setup::operator ==(com_sim_setup& a)
{
  bool returnme = true;
  // check the singular variables
  returnme = (name == a.name) and returnme;
  returnme = (model == a.model) and returnme;
  returnme = (quanta_ms == a.quanta_ms) and returnme;
  returnme = (max_quantas == a.max_quantas) and returnme;
  returnme = (started_by == a.started_by) and returnme;
  // check the items listed.
  if (items.size() != a.items.size())
  {
	// failed for sure.
	returnme = false;
  }
  else 
  {
	// check them to see if they are the same.
	for (int i=0; i<items.size(); i++)
	{
	  returnme = (*items[i] == *a.items[i]) and returnme;
	};
  };
  return returnme;
};
  
} // namespace nrtb::com

