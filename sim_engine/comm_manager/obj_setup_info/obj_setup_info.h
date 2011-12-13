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
 
#ifndef obj_setup_header
#define obj_setup_header

#include <location_data.h>
#include <velocity_data.h>
#include <sim_management.pb.h>

namespace nrtb_com {

class com_obj_setup
{
public:
  uint32_t uid;
  std::string name;
  uint32_t obj_type;
  com_location location;
  com_velocity velocity;
  
  com_obj_setup();
  com_obj_setup(nrtb_msg::obj_setup_data * ext);
  virtual ~com_obj_setup() {};

  void set_from_message(nrtb_msg::obj_setup_data* ext);
  void load_message(nrtb_msg::obj_setup_data* ext);  
  
  bool operator ==(com_obj_setup & a);
};

} // namespace nrtb::com

#endif // triplet_header
