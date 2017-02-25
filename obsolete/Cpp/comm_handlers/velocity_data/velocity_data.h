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
 
#ifndef velocity_header
#define velocity_header

#include <triplet.h>
#include <physics_common.pb.h>

namespace nrtb_com {

class com_velocity
{
public:
  com_triplet speed;
  com_triplet rotation;
  
  com_velocity();
  com_velocity(nrtb_msg::velocity_data * ext);
  virtual ~com_velocity() {};
  
  void set_from_message(nrtb_msg::velocity_data* ext);
  void load_message(nrtb_msg::velocity_data* ext);

  bool operator ==(com_velocity & a);
};

} // namespace nrtb::com

#endif // triplet_header
