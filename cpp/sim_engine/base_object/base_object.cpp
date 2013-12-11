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

#include "base_object.h"
#include <sstream>

using namespace nrtb;

bool base_object::tick(int time)
{
  force = 0;
  torque = 0;
  mass_mod = 0;
  bool killme (false);
  for (auto e : pre_attribs)
    if (e.second->tick(*this, time))
      killme = true;
  return killme;
};

bool base_object::apply(int time, float quanta)
{
  // move acording to forces
  float tmass = mass + mass_mod;
  triplet a = force / tmass;
  triplet ra = torque / (tmass * 0.5); // not accurate!!
  velocity += (a * quanta) * accel_mod;
  rotation += (ra * quanta) * torque_mod;
  location += velocity * quanta;
  attitude += rotation * quanta;
  // apply post-effectors
  bool killme (false);
  for (auto e : post_attribs)
    if (e.second->tick(*this, time))
      killme = true;
  return killme;
};

bool base_object::check_collision(sphere s)
{
  float r = s.radius + bounding_sphere.radius;
  return r <= s.center.range(bounding_sphere.center);
};
