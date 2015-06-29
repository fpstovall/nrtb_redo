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

  
/***************************************************
 * diff_steer provides the effetors and control 
 * inteface for a differential drive sytsem. This is 
 * good for modeling a tank or any other ground 
 * friction based system which steers by applying 
 * unequal forces between the left and right sides.
 **************************************************/  
class diff_steer
{
public:
  /*************************************************
   * Contructor which establishes all the constants
   * for the diff_steer module. Once established there
   * is no means to change or report these constants.
   *  - o : the base_object descendent this module 
   *      affects. Normally, this diff_steer would 
   *      be a member of the provided object.
   *  - thrust : the maximum thrust, in newtons, 
   *      which this drive system can apply to the 
   *      object.
   *  - _break : The maximun braking force in newtons
   *      which can be applyed to the object.
   *  - turn_rate : the maxium turn rate which the 
   *      object can achieve, in radians.
   *  -  skid_threshold and slide_friction are not 
   *      used in this implementation and are 
   *      resereved for future use.
   ************************************************/
  diff_steer(base_object & o, float thrust, float _brake,
             float turn_rate,
             float skid_threshold, float slide_friction); 
  // control methods -- illegal valus force full stop.
  // Sets the drive power level, 0.0 >= power >= 1.0
  float drive(float power);
  // Sets the braking level, 0.0 >= braking >= 1.0
  float brake(float braking);
  // Sets the turn rate, 0.0 >= rate >= 1.0
  float turn(float rate);
  // Full stop; power 0, braking 1, turn 0.
  void lockdown();        
  // status reporting;
  float get_drive();  // returns 0.0 - 1.0
  float get_brake();  // returns 0.0 - 1.0
  float get_turn();   // returns 0.0 - 1.0
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
