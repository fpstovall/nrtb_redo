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

#include "obj_setup_info.h"

namespace nrtb_com
{

com_obj_setup::com_obj_setup()
{
  name = "uninitialized";
  uid = 0;
  obj_type = 0;
};

com_obj_setup::com_obj_setup(nrtb_msg::obj_setup_data* ext)
{
  set_from_message(ext);
};

void com_obj_setup::set_from_message(nrtb_msg::obj_setup_data* ext)
{
  uid = ext->uid();
  name = ext->name();
  obj_type = ext->obj_type();
  location.set_from_message(ext->mutable_location());
  velocity.set_from_message(ext->mutable_velocity());;
};

void com_obj_setup::load_message(nrtb_msg::obj_setup_data* ext)
{
  ext->set_uid(uid);
  ext->set_name(name);
  ext->set_obj_type(obj_type);
  location.load_message(ext->mutable_location());
  velocity.load_message(ext->mutable_velocity());
};

bool com_obj_setup::operator ==(com_obj_setup& a)
{
  bool returnme = true;
  returnme = (a.location == location) && returnme;
  returnme = (a.velocity == velocity) && returnme;
  returnme = (a.uid == uid) && returnme;
  returnme = (a.name == name) && returnme;
  returnme = (a.obj_type == obj_type) && returnme;
  return returnme;
};
  
} // namespace nrtb::com

