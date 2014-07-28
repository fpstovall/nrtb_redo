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


#include "triplet.h"
#include "velocity_data.h"

namespace nrtb_com
{

com_velocity::com_velocity() {};

  
com_velocity::com_velocity(nrtb_msg::velocity_data * ext)
{
  set_from_message(ext);
};

void com_velocity::set_from_message(nrtb_msg::velocity_data* ext)
{
  speed.set_from_message(ext->mutable_speed());
  rotation.set_from_message(ext->mutable_rotation());
};

void com_velocity::load_message(nrtb_msg::velocity_data* ext)
{
  speed.load_message(ext->mutable_speed());
  rotation.load_message(ext->mutable_rotation());
};

bool com_velocity::operator==(com_velocity& a)
{
  bool returnme = true;
  returnme = (speed == a.speed) and returnme;
  returnme = (rotation == a.rotation) and returnme;
  return returnme;
};

} // namespace nrtb::com

