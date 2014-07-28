/***********************************************
 This file is part of the NRTB project (https://github.com/fpstovall/nrtb_alpha).
 
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
 
#ifndef tq_obj_update_header
#define tq_obj_update_header

#include <location_data.h>
#include <velocity_data.h>
#include <sim_obj_tq_update.pb.h>

namespace nrtb_com {

class com_tq_obj_update
{
public:
  uint32_t obj_uid;
  com_location location;
  com_velocity velocity;
  nrtb::strlist event;
  nrtb::strlist inbound_com;
  nrtb::strlist outbound_com;
  
  com_tq_obj_update();
  com_tq_obj_update(nrtb_msg::sim_obj_tq_data * ext);
  virtual ~com_tq_obj_update() {};

  void set_from_message(nrtb_msg::sim_obj_tq_data* ext);
  void load_message(nrtb_msg::sim_obj_tq_data* ext);  
  
  bool operator ==(com_tq_obj_update & a);
};

} // namespace nrtb::com

#endif // triplet_header
