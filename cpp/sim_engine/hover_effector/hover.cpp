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

// see base_socket.h for documentation

#include "hover.h"
#include <sstream>
#include <iostream>

using namespace nrtb;

hover::hover(float set, float _range, float bias)
  : set_altitude(set), range(_range), curve(bias)
{};

effector_p hover::clone()
{
  return effector_p(new hover(*this));
};

std::string hover::as_str()
{
  std::stringstream s;
  s << "hover=" << set_altitude << "," 
    << range << "," << curve;
  return s.str();
};

bool hover::tick(base_object& o, float quanta)
{
  float deviation = fabs(set_altitude - o.location.z);
  // override assumed gravity if we are below setting.
  if (o.location.z < set_altitude)
  { 
    o.accel_mod.z += 9.80665 * 2;
  };
  // damp verticals near the set point.
  float damp = powf(1.0 - deviation/range,curve);
  o.velocity.z *= (damp > 0.9) ? 0.9 : damp;
  return false;
};

