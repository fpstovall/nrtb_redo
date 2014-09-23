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
  lockdown();
};

float diff_steer::drive(float power)
{
  if (fabsf(power) <= 100.0)
  {
    pre_effector->set_p.store(power/100.0);
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
    pre_effector->set_b.store(braking/100.0);
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
    pre_effector->set_t.store(rate/100.0);
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
    // Apply propulsion force.
    o.force += (vec * p);
    // get the kenetic energy.
    triplet vel = o.velocity;
    vel.z = 0.0;
    float speed = vel.magnatude();
    float bspeed = speed * 50;
    float KE = o.mass * bspeed * bspeed ;
    // Include current power setting in budget.
    KE += p;
    // limit b to KE
    b = (b > KE) ? KE : b;
    // apply breaking force.
    o.force -= (vec * b);
    // Apply turn settings
    o.torque.add(t);
    // apply breaking effects to turn rate
    float rV = o.rotation.angles().z;
    float rKE = (o.mass * rV * rV)/2.0;
    // limit braking torque to rKE if needed. 
    float rb = (b > rKE) ? rKE : b;
    // apply the braking force opposite rotation.
    o.torque.add((rV > 0.0 ? -rb : rb));
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
    // get velocity vector and speed.
    triplet DoT = o.velocity;
    float speed = o.velocity.magnatude();
    // -- squash verticals
    DoT.z = 0.0;
    // get the current ground speed
    float gspeed = DoT.magnatude();
    // build the xy plane vector
    rotatable & a = o. attitude;
    triplet DoH(a.get_cos().z,a.get_sin().z,0.0);
    // get the cosine of the angle between them.
    DoT = DoT.normalize();
    float delta = DoT.dot_product(DoH);
    // are we sliding?
    if ((gspeed > 0.0001) and (delta < 0.995))
    {
      /********************************
       * sliding.. need to fix and apply drag.
       * simplistic for alpha.. simply snap to the 
       * current heading.
       *******************************/
      // Scale new xy to match original components
      DoH = DoH * gspeed;
      // restore the z component.
      DoH.z = o.velocity.z;
      // TODO: we may want remove this check.
      // Verify we have the correct velocity.
      if (fabs(DoH.magnatude() - speed) > 0.001)
      {
        // Sanity check failed.
        base_exception e;
        std::stringstream s;
        s << "diff_steer::post::tick() DoH magnitude incorrect"
          << ":" << DoH << "," << DoH.magnatude();
        e.store(s.str());
        throw e;
      };
      // scale back to original speed adjusted for slide drag
      o.velocity = DoH * delta;
    };
    // if gspeed is less than 0.001, just stop.
    if (gspeed < 1e-3)
    {
      o.velocity.x = 0.0;
      o.velocity.y = 0.0;
    };
    /******** removed for now **********
    // apply rolling friction (limit is 360 km/h).
    // calculate drag
    float drag_q = 1.0 - (gspeed/10000.0);
    drag_q = drag_q > 1.0 ? 1.0 : drag_q;
    // assemble drag vector;
    triplet drags(drag_q,drag_q,1.0);
    // apply drag.
    o.velocity = o.velocity * drags;
    ***********************************/
  };
  return false;
};


