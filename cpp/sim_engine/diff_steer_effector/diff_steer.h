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
 
#ifndef diff_steer_effector_header
#define diff_steer_effector_header

#include <triad.h>
#include <base_object.h>

namespace nrtb
{
/****************************************
 * WARNING: Never use the effectors below
 * directly. Instead, instanciate an instance
 * of the diff_steer class found later and
 * use that to provide the effectors to the
 * object and your control interface into
 * the effectors.
 ****************************************/  

struct diff_steer_pre: public abs_effector
{
  // TODO: Constructor
  // TODO: local data
  // required overrides.
  abs_effector * clone();
  std::string as_str();
  bool tick(base_object & o, int time);
};

struct diff_steer_post: public abs_effector
{
  // TODO: Constructor
  // TODO: local data
  // required overrides.
  abs_effector * clone();
  std::string as_str();
  bool tick(base_object & o, int time);
};

typedef std::shared_ptr<diff_steer_pre> df_pre_p;
typedef std::shared_ptr<diff_steer_post> df_post_p;

class diff_steer
{
public:
  diff_steer(base_object & o, float thrust, float turn_rate,
             float skid_friction, float slide_friction); 
  // control methods
  float drive(float power);   // sets the "throttle"
  float brake(float braking); // sets the brake
  float turn(float rate);     // sets turn rate
  // status reporting;
  float get_drive();
  float get_brake();
  float get_turn();
protected:
  float drive_val {0.0};
  float brake_val {0.0};
  float turn_val  {0.0};
  df_pre_p pre_effector;
  df_post_p post_effector;  
};


} // namepace nrtb

#endif // diff_steer_effector_header
