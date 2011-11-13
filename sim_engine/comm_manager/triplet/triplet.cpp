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


#include "triplet.h"

namespace nrtb_com
{

com_triplet::com_triplet(triplet in)
{
  set(in);
};

com_triplet::com_triplet(nrtb_msg::triplet* ext)
{
  set_from_message(ext);
};

void com_triplet::set(triplet in)
{
  internal = in;
};

triplet com_triplet::get()
{
  return internal;
};

void com_triplet::set_from_message(nrtb_msg::triplet* ext)
{
  internal = 0;
  internal.x = ext->x(); 
  internal.y = ext->y();
  internal.z = ext->z();
};

void com_triplet::load_message(nrtb_msg::triplet* ext)
{
  ext->set_x(internal.x);
  ext->set_y(internal.y);
  ext->set_z(internal.z);
};



  
} // namespace nrtb::com

