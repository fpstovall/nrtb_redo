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
 
#ifndef hover_effector_header
#define hover_effector_header

#include <triad.h>
#include <base_object.h>

namespace nrtb
{

/*******************************************
 * hover provides an effector which strives 
 * to keep the attached object at a specified
 * altitude. Value set at construction control
 * how much force the effector can apply and 
 * how closely the effector tracks the set
 * altitude. 
 ******************************************/  
struct hover: public abs_effector
{
  /*******************************************
  * Constructs with the settings for altitude,
  * allowed drift range, and a scaling factor 
  * debounce.
  * - set is the target altitude.
  * - _range sets an allowable drift range.
  * -  bias controls damping near the target.
  *******************************************/
  hover(float set, float _range, float bias);
  // target ride height.
  float set_altitude;
  // acceptable range
  float range;
  // exponent to bias response.
  float curve;
  // required overrides.
  effector_p clone();
  std::string as_str();
  // tick() does the math.
  bool tick(base_object & o, float quanta);
};
  
} // namepace nrtb

#endif // hover_effector_header
