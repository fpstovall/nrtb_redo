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

class diff_steer
{
public:
  diff_steer(base_object & o, float thrust, float _brake,
             float turn_rate,
             float skid_threshold, float slide_friction); 
  // control methods
  float drive(float power);   // sets the "throttle"
  float brake(float braking); // sets the brake
  float turn(float rate);     // sets turn rate
  void lockdown();            // full stop.
  // status reporting;
  float get_drive();
  float get_brake();
  float get_turn();
  // effector definitions
  struct pre: public abs_effector
  {
    // Constructor
    pre(float mp, float mb, float mt);
    pre(const pre & t);
    // local data
    float max_p;
    float max_t;
    float max_b;
    // Input fields
    std::atomic<float> set_p {0.0};
    std::atomic<float> set_t {0.0};
    std::atomic<float> set_b {1.0};
    // required overrides.
    effector_p clone();
    std::string as_str();
    bool tick(base_object & o, float quanta);
  };
  struct post: public abs_effector
  {
    // Constructor
    post(float t, float f);
    post(const post & t);
    // local data
    float skid_threshold;
    float slide_friction;
    // required overrides.
    effector_p clone();
    std::string as_str();
    bool tick(base_object & o, float quanta);
  };
  typedef std::shared_ptr<pre> pre_p;
  typedef std::shared_ptr<post> post_p;
protected:
  pre_p pre_effector;
  post_p post_effector;  
};

} // namepace nrtb

#endif // diff_steer_effector_header
