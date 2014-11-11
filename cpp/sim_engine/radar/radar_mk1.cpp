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

// see base_socket.h for documentation

#include "radar_mk1.h"
#include <sstream>
#include <iostream>

using namespace nrtb;

radar_mk1::radar_mk1(base_object & o)
  : sim(global_sim_core::get_reference()),
    parent(o)
{};

std::string radar_mk1::contacts()
{
  std::stringstream returnme;
  // for now, update every time.
  c_list = sim.contact_list();
  // assemble the return string
  returnme << c_list->size() -1 << " ";
  for(auto c : *c_list)
  {
    if (c.id != parent.id)
    {
      float range = parent.location.range(c.location);
      // get xy azimuth
      triplet offset = parent.location - c.location;
      float azimuth = atan2(offset.y, offset.x);
      // get yz elevation
      float elevation = atan2(offset.z, offset.x);
      // assemble return string
      returnme << c.type << " "
        << range << " "
        << azimuth << " "
        << elevation << " "
        << c.velocity << " ";
    };
  };
  return returnme.str();
};

std::string radar_mk1::status()
{
  return "1";
};
