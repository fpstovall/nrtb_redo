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

#include "hover.h"
#include <sstream>

using namespace nrtb;

hover::hover(float set, float min, float bias)
  : set_altitude(set), range(set-min), curve(bias)
{};

abs_effector * hover::clone()
{
  return new hover(*this);
};

std::string hover::as_str()
{
  std::stringstream s;
  s << "hover=" << set_altitude << "," 
    << range << "," << curve;
  return s.str();
};

bool hover::tick(base_object& o, int time)
{
  const int max_lift(9.80665 * 100);
  // do nothing if we are above set_altitude
  if (o.location.z < set_altitude)
  {
    float deviation = set_altitude - o.location.z;
    float adjustment = pow(deviation/range,curve);
    o.accel_mod += adjustment;
  };
  return false;
};

