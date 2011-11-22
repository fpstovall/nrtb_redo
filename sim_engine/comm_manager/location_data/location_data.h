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
 
#ifndef location_header
#define location_header

#include <triplet.h>
#include <physics_common.pb.h>

namespace nrtb_com {

class com_location
{
public:
  com_location();
  com_location(com_triplet pos, com_triplet att );
  com_location(nrtb_msg::location_data * ext);
  virtual ~com_location() {};
  
  void set(com_triplet pos, com_triplet att);
  com_triplet get_pos();
  com_triplet get_att();
  void set_from_message(nrtb_msg::location_data* ext);
  void load_message(nrtb_msg::location_data* ext);
  
protected:
  com_triplet position;
  com_triplet attitude;
};

} // namespace nrtb::com

#endif // triplet_header
