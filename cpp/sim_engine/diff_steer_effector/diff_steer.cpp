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

#include "diff_steer.h"
#include <sstream>
#include <iostream>

using namespace nrtb;

diff_steer::diff_steer(base_object& o, float thrust, float _brake,
                       float turn_rate, 
                       float skid_threshold, float slide_friction)
{
//  pre_effector.reset(new pre(thrust,_brake,turn_rate));
//  post_effector.reset(new post(skid_threshold,slide_friction));
//  effector_p t(pre_effector);
  o.add_pre(std::make_shared<pre>(thrust,_brake,turn_rate));
//  t = post_effector;
  o.add_post(std::make_shared<post>(skid_threshold,slide_friction));
};

/******** pre-effector **********/

diff_steer::pre::pre(float mp, float mb, float mt)
  : max_p(mp), max_b(mb), max_t(mt) {};

diff_steer::pre::pre(const diff_steer::pre& t)
  : max_p(t.max_p), max_b(t.max_b), max_t(t.max_t),
  set_p(t.set_p.load()), set_b(t.set_b.load()),set_t(t.set_t.load())
{
	lockdown();
};

effector_p diff_steer::pre::clone()
{
  return effector_p(new diff_steer::pre(*this));
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

float diff_steer::pre::drive(float power)
{
  if (fabsf(power) <= 100.0)
  {
    if (power != 0.0) set_b.store(0.0);
    set_p.store(power/100.0);
  }
  else
  {
    lockdown();
  };
  return power;
};

float diff_steer::pre::brake(float braking)
{
  if ((braking >= 0.0) and (braking <= 100.0))
  {
    if (braking != 0.0) set_p.store(0.0);
    set_b.store(braking/100.0);
  }
  else 
  {
    lockdown();
  };
  return braking;
};

float diff_steer::pre::turn(float rate)
{
  if (fabsf(rate) <= 100.0)
  {
    set_t.store(rate/100.0);
  }
  else
  {
    lockdown();
  };
  return rate;
};

void diff_steer::pre::lockdown()
{
  // full stop.
  set_p.store(0.0);
  set_b.store(1.0);
  set_t.store(0.0);  
};

float diff_steer::pre::get_drive()
{
  return set_p.load();
};

float diff_steer::pre::get_brake()
{
  return set_b.load();
};

float diff_steer::pre::get_turn()
{
  return set_t.load();
};

bool diff_steer::pre::command(std::string cmd, std::string & response)
{
  bool returnme = false;
  std::stringstream tokens(cmd);
  std::string sys;
  std::string verb;
  tokens >> sys >> verb;
  // check for drive commands
  response = "";
  if (sys == "drive")
  {
    returnme = true;
    if (verb == "status")
    {
      std::stringstream s;
      s << sys << " " << verb << " "
        << get_drive()
        << " " << get_brake()
        << " " << get_turn();
      response = s.str();
    }
    else if (!tokens.eof())
    {
      // get the float argument.
      float val;
      tokens >> val;
      // apply val to the correct setting.
      if (verb == "power") { drive(val); }
      else if (verb == "brake") { brake(val); }
      else if (verb == "turn") { turn(val); };
    }
    else 
    {
      response = "bad_cmd \""+cmd+"\"";
      lockdown();
    };
  };
  return returnme;
};

bool diff_steer::pre::tick(base_object& o, float quanta)
{
  // are we in contact with the ground?
  float gl = o.location.z  - o.bounding_sphere.radius;
  if (gl < 0.5)
  {
    float p = set_p.load() * max_p;
    float b = set_b.load() * max_b;
    float t = set_t.load() * max_t;
    // Adjust our turn rate 
    triplet rot = o.rotation.angles();
    rot.z = t;
    o.rotation.set(rot);
    // calc the planar thrust and brake vector
    rotatable & a = o.attitude;
    triplet vec(a.get_cos().z,a.get_sin().z,0.0);
    // Apply propulsion force.
    o.force += (vec * p);
    // get the kenetic energy.
    triplet vel = o.velocity;
    vel.z = 0.0;
    float speed = vel.magnatude();
    float bspeed = speed * (1.0/quanta);
    float KE = o.mass * bspeed * bspeed ;
    // Include current power setting in budget.
    KE += p;
    // limit b to KE
    b = (b > KE) ? KE : b;
    // apply breaking force.
    o.force -= (vec * b);
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

effector_p diff_steer::post::clone()
{
  return effector_p(new diff_steer::post(*this));
};

std::string diff_steer::post::as_str()
{
  std::stringstream s;
  s << "diff_steer::post=" 
    << skid_threshold << "," << slide_friction;
  return s.str();
};

bool diff_steer::post::tick(base_object& o, float time)
{
  // are we in contact with the ground?
  float gl = o.location.z  - o.bounding_sphere.radius;
  if (gl < 0.5)
  {
    // get velocity vector and speed.
    triplet DoT = o.velocity;
    triplet att = o.attitude.angles();
    // -- squash verticals
    DoT.z = 0.0;
    // get the current ground speed and direction
    triplet gnd_vec = DoT.to_polar();
    if (gnd_vec.y < 0.0) gnd_vec.y += 2*pi;
    float delta = fabs(gnd_vec.y - att.z) ;
    // are we sliding?
    if ((gnd_vec.x > 0.0001) and (delta > 0.005))
    {
      /********************************
       * sliding.. need to fix and apply drag.
       * simplistic for alpha.. simply snap to the 
       * current heading.
       *******************************/
      gnd_vec.y = att.z; 
      // adjust gspeed to deal with the skid.
      gnd_vec.x *= 1.0 - pow(delta * (gnd_vec.x/1e4), 3);
      // store the changes back to the object.
      DoT = gnd_vec.to_cartesian();
      o.velocity.x = DoT.x;
      o.velocity.y = DoT.y;
    };
    // if gspeed is less than 0.001, just stop.
    if (gnd_vec.x < 1e-3)
    {
      o.velocity.x = 0.0;
      o.velocity.y = 0.0;
      gnd_vec.x = 0.0;
    };
    // apply rolling friction (limit is 360 km/h).
    // calculate drag
    float drag_q = 1.0 - pow(gnd_vec.x/1e4,2);
    drag_q = drag_q > 1.0 ? 1.0 : drag_q;
    // assemble drag vector;
    triplet drags(drag_q,drag_q,1.0);
    // apply drag.
    o.velocity = o.velocity * drags;
  };
  return false;
};


