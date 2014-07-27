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
#include "location_data.h"

namespace nrtb_com
{

com_location::com_location() {};

  
com_location::com_location(nrtb_msg::location_data * ext)
{
  set_from_message(ext);
};

void com_location::set_from_message(nrtb_msg::location_data* ext)
{
  position.set_from_message(ext->mutable_position());
  attitude.set_from_message(ext->mutable_attitude());
};

void com_location::load_message(nrtb_msg::location_data* ext)
{
  position.load_message(ext->mutable_position());
  attitude.load_message(ext->mutable_attitude());
};

bool com_location::operator==(com_location& a)
{
  bool returnme = true;
  returnme = (position == a.position) and returnme;
  returnme = (attitude == a.attitude) and returnme;
  return returnme;
}
  
} // namespace nrtb::com

