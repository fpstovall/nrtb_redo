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
  pre_effector.reset(new pre(thrust,_brake,turn_rate));
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
    pre_effector->set_b.store(braking/100);
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
  pre_effector->set_b.store(1.0);
  pre_effector->set_t.store(0.0);  
};

float diff_steer::get_drive()
{
  return pre_effector->set_p.load();
};

float diff_steer::get_brake()
{
  return pre_effector->set_b.load();
};

float diff_steer::get_turn()
{
  return pre_effector->set_t.load();
};

/******** pre-effector **********/

diff_steer::pre::pre(float mp, float mb, float mt)
  : max_p(mp), max_b(mb), max_t(mt) {};

diff_steer::pre::pre(const diff_steer::pre& t)
  : max_p(t.max_p), max_b(t.max_b), max_t(t.max_t),
  set_p(t.set_p.load()), set_b(t.set_b.load()),set_t(t.set_t.load())
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
    << max_b << "," << set_b << ","
    << max_t << "," << set_t;
  return s.str();
};

bool diff_steer::pre::tick(base_object& o, int time)
{
  // TODO: Add braking here.
  // are we in contact with the ground?
  float gl = o.location.z  - o.bounding_sphere.radius;
  if (gl < 0.5)
  {
    float p = set_p.load() * max_p;
    float b = set_b.load() * max_b;
    float t = set_t.load() * max_t;
    // calc the planar thrust and brake vector
    rotatable & a = o.attitude;
    triplet vec(a.get_cos().z,a.get_sin().z,0.0);
    // Apply propulsion and braking setting.
    o.accel_mod += (vec * p);
    o.accel_mod -= (vec * b);
    // Apply turn settings
    o.torque.add(t);
    o.torque.add(-(b/100.0));
  };
  return false;
};

/******** post-effector **********/

diff_steer::post::post(float t, float f)
  : skid_threshold(t), slide_friction(f)
{};

diff_steer::post::post(const diff_steer::post& t)
  : slide_friction(t.slide_friction),
    skid_threshold(t.skid_threshold)
{};

abs_effector * diff_steer::post::clone()
{
  return new diff_steer::post(*this);
};

std::string diff_steer::post::as_str()
{
  std::stringstream s;
  s << "diff_steer::post=" 
    << skid_threshold << "," << slide_friction;
  return s.str();
};

bool diff_steer::post::tick(base_object& o, int time)
{
  // are we in contact with the ground?
  float gl = o.location.z  - o.bounding_sphere.radius;
  if (gl < 0.5)
  {
    // get velocity vector.
    triplet DoT = o.velocity.normalize();
    // save some copies separately.. we'll need it later.
    triplet tv = DoT;
    // -- squash verticals
    DoT.z = 0.0;
    DoT.normalize();
    // build the xy plane vector
    rotatable & a = o. attitude;
    triplet DoH(a.get_cos().z,a.get_sin().z,0.0);
    // get the cosine of the angle between them.
    float delta = DoT.dot_product(DoH);
    // are we sliding?
    if (delta < 0.995)
    {
      // sliding.. need to fix and apply drag.
      // simplistic for alpha.. simply snap to the 
      // get the current speed.
      float speed = o.velocity.magnatude();
      // Scale new xy to match original components
      float xy_scale = 1.0 - ((tv.x*tv.x)+(tv.y*tv.y));
      DoH = DoH * xy_scale;
      // restore the z component.
      DoH.z = tv.z;
      // TODO: remove this check once we know things work.
      // Verify this is a unit vector.
      if (fabs(DoH.magnatude() - 1.0) > 0.001)
      {
        // Sanity check failed.
        base_exception e;
        e.store("diff_steer::post::tick() DoH magnitude != 1.0");
        throw e;
      };
      // TODO: Complete applying corrections.
      // scale back to original speed adjusted for slide drag
      o.velocity = DoH * (speed * delta);
    };
  };
  // apply rolling friction (limit is 360 km/h).
  float speed = o.velocity.magnatude();
  float drag_q = 1 - ((speed*speed)/10000);
  o.velocity = o.velocity * (drag_q > 0.0 ? drag_q : 0.0);
  return false;
};


