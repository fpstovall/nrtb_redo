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

#include "diff_steer.h"
#include <sstream>
#include <iostream>

using namespace nrtb;

diff_steer::diff_steer(base_object& o, float thrust, float _brake,
                       float turn_rate, 
                       float skid_threshold, float slide_friction)
{
  pre_effector.reset(new pre(thrust,turn_rate));
  post_effector.reset(new post(skid_threshold,slide_friction));
  effector_p t(pre_effector);
  o.add_pre(t);
  t = post_effector;
  o.add_post(t);
};

float diff_steer::drive(float power)
{
  if (fabsf(power) <= 100.0)
  {
    pre_effector->set_p.store(power/100);
  }
  else
  {
    lockdown();
  };
};

float diff_steer::brake(float braking)
{
  if ((braking >= 0.0) and (braking <= 100.0))
  {
//    pre_effector->set_b.store(braking/100);
  }
  else 
  {
    lockdown();
  };
};

float diff_steer::turn(float rate)
{
  if (fabsf(rate) <= 100.0)
  {
    pre_effector->set_t.store(rate/100);
  }
  else
  {
    lockdown();
  };
};

void diff_steer::lockdown()
{
  // full stop.
  pre_effector->set_p.store(0.0);
//  pre_effector->set_b.store(1.0);
  pre_effector->set_t.store(0.0);  
};

float diff_steer::get_drive()
{
  return pre_effector->set_p.load();
};

float diff_steer::get_brake()
{
//  return pre_effector->set_b.load();
};

float diff_steer::get_turn()
{
  return pre_effector->set_t.load();
};

/******** pre-effector **********/

diff_steer::pre::pre(float mp, float mt)
  : max_p(mp), max_t(mt) {};

diff_steer::pre::pre(const diff_steer::pre& t)
  : max_p(t.max_p), max_t(t.max_t),
  set_p(t.set_p.load()), set_t(t.set_t.load())
{};

abs_effector * diff_steer::pre::clone()
{
  return new diff_steer::pre(*this);
};

std::string diff_steer::pre::as_str()
{
  std::stringstream s;
  s << "diff_steer::pre=" 
    << max_p << "," << set_p << ","
    << max_t << "," << set_t;
  return s.str();
};

bool diff_steer::pre::tick(base_object& o, int time)
{
  // are we in contact with the ground?
  float gl = o.location.z  - o.bounding_sphere.radius;
  if (gl < 0.5)
  {
    float p = set_p.load() * max_p;
    float t = set_t.load() * max_t;
    // calc the planar thrust and brake vector
    triplet vec = o.attitude;
    vec.z = 0.0;
    vec = vec.normalize();
    // Apply propulsion setting.
    o.accel_mod += (vec * p);
    // Apply turn setting;
    o.torque_mod.z += t;    
  };
  return false;
};

/******** post-effector **********/

diff_steer::post::post(float t, float f)
  : skid_threshold(t), slide_friction(f)
{};

abs_effector * diff_steer::post::clone()
{
  return new diff_steer::post(*this);
};

std::string diff_steer::post::as_str()
{
  std::stringstream s;
  // TODO: Complete the status string
  s << "diff_steer::post=" << "TODO";
  return s.str();
};

bool diff_steer::post::tick(base_object& o, int time)
{
  // TODO: just a filler.
  return false;
};


