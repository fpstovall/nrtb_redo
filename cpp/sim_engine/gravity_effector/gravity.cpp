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

// see base_socket.h for documentation

#include "gravity.h"
#include <sstream>

using namespace nrtb;

abs_effector* norm_gravity::clone()
{
  return new norm_gravity(*this);
};

std::string norm_gravity::as_str()
{
  std::stringstream s;
  s << "gravity=" << gn;
  return s.str();
};

bool norm_gravity::tick(base_object& o, int time)
{
  o.accel_mod += gn;
  return false;
};

