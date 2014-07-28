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
 
#ifndef hover_effector_header
#define hover_effector_header

#include <triad.h>
#include <base_object.h>

namespace nrtb
{

struct hover: public abs_effector
{
  hover(float set, float min, float bias);
  // target ride height.
  float set_altitude;
  // min ride height.
  float min_altitude;
  float range;
  // exponent to bias response.
  float curve;
  // required overrides.
  abs_effector * clone();
  std::string as_str();
  bool tick(base_object & o, int time);
};
  
} // namepace nrtb

#endif // hover_effector_header
