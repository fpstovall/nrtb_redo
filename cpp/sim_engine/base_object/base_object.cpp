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

// see base_object.h for documentation

#include "base_object.h"
#include <sstream>

using namespace nrtb;

serializer abs_effector::effector_num;
serializer base_object::object_num;

rotatable::rotatable(triplet s)
  : axis(s), dirty(true) 
{};

rotatable::rotatable(rotatable & a)
  : axis(a.angles()), dirty(true)
{};

void rotatable::trim()
{
  dirty = true;
  axis.x = fmodf(axis.x,period);
  axis.y = fmodf(axis.y,period);
  axis.z = fmodf(axis.z,period);
};

void rotatable::apply_force(float mass, float arm, 
                            triplet vec, float t)
{
  dirty = true;
  float I = (((arm*arm) * mass)/2);
  axis.x += (vec.x / I) * t;
  axis.y += (vec.y / I) * t;
  axis.z += (vec.z / I) * t;
};

void rotatable::scale(triplet factor)
{
  dirty = true;
  axis *= factor;
};

void rotatable::add(triplet value)
{
  dirty = true;
  axis += value;
};

void rotatable::set(triplet a)
{
  dirty = true;
  axis = a;
};

void rotatable::set(rotatable a)
{
  set(a.angles());
};

triplet rotatable::angles()
{
  return axis;
};

triplet rotatable::get_cos()
{
  if (dirty) recalc();
  return cos;
};

triplet rotatable::get_sin()
{
  if (dirty) recalc();
  return sin;
};

void rotatable::recalc()
{
  cos.x = cosf(axis.x);
  cos.y = cosf(axis.y);
  cos.z = cosf(axis.z);
  sin.x = sinf(axis.x);
  sin.y = sinf(axis.y);
  sin.z = sinf(axis.z);
  dirty=false;
};

void effector_list::add(effector_p e)
{
  insert(std::pair<unsigned long long, effector_p>(e->id, e));
};

void effector_list::remove(effector_p e)
{
  erase(e->id);
};

void effector_list::remove(long long unsigned int key)
{
  erase(key);
};

std::string base_object::as_str()
{
  std::stringstream returnme;
  returnme << "ID=" << id
    << ":loc=" << location
    << ":att=" << attitude.angles()
    << ":vel=" << velocity
    << ":rot=" << rotation.angles()
    << ":f=" << force
    << ":t=" << torque.angles()
    << ":acc_mod=" << accel_mod
    << ":r_mod=" << rotation_mod.angles()
    << ":mass=" << mass
    << ":mass_mod=" << mass_mod
    << ":b_sphere=" << bounding_sphere.center
    << "," << bounding_sphere.radius
    << ":pre=";
  for(auto a : pre_attribs)
    returnme << a.second->as_str() << ";";
  returnme << ":posts=";
  for(auto a : post_attribs)
    returnme << a.second->as_str() << ";";
  return returnme.str();
};

bool base_object::tick(float quanta)
{
  // clean up for next pass
  accel_mod = 0;
  rotation_mod.set(triplet(0));
  force = 0;
  torque.set(triplet(0));
  mass_mod = 0;
  // execute any pending attrib drops
  for(auto i : dropped_attribs)
  {
    // ignore errors here
    try
    {
      pre_attribs.remove(i);
      post_attribs.remove(i);
    }
    catch (...) {};
  };
  dropped_attribs.clear();
  // execute the pre_attribs list.
  bool killme (false);
  for (auto e : pre_attribs)
    if (e.second->tick(*this, quanta))
      killme = true;
  // apply forces to rotation, mass, and velocity.
  mass += mass_mod;
  triplet a = force / mass;
  triplet ev = velocity + (((a + accel_mod)/2.0) * quanta);
  velocity += (a  + accel_mod) * quanta;
  rotatable rbase(rotation);
  rotation.add(rotation_mod.angles() * quanta);
  rotation.apply_force(mass, bounding_sphere.radius,
                       torque.angles(),quanta);
  rotation_mod.set((rotation.angles() + rbase.angles()) / 2.0);
  return killme;
};

bool base_object::apply(float quanta)
{
  // move acording to forces
  location += velocity * quanta;
  // rotate according to the forces
  attitude.add(rotation_mod.angles() * quanta);
  attitude.trim();
  // apply post-effectors
  bool killme (false);
  for (auto e : post_attribs)
    if (e.second->tick(*this, quanta))
      killme = true;
  return killme;
};

bool base_object::check_collision(object_p o, float quanta)
{
  // get relative velocity and position
  triplet rel_vel = o->velocity - velocity;
  triplet rel_pos = o->location - location;
  // adjust rel_vel for time quanta
  rel_vel *= quanta;
  // contact distance
  float r = bounding_sphere.radius + o->bounding_sphere.radius;
  // dP^2-r^2
  float pp = (rel_pos.x*rel_pos.x) + (rel_pos.y*rel_pos.y) 
    + (rel_pos.z*rel_pos.z) - (r*r);
  // Are we already interescting?
  if (pp <= 0.0) return true;
  
  // DP*dV
  float pv = (rel_pos.x*rel_vel.x) + (rel_pos.y*rel_vel.y)
    + (rel_pos.z*rel_vel.z);
  // check to see if the objects are moving apart.
  if (pv >= 0.0) return false;
    
  // dv ^2
  float vv = (rel_vel.x*rel_vel.x) + (rel_vel.y*rel_vel.y)
    + (rel_vel.z*rel_vel.z);
  // do we have contact within the quanta?
  if (((pv+vv) <= 0.0) and ((vv+2*pv+pp) >= 0.0)) return false;
  
  // What time are these closest?
  float min_time = pv/vv;

  return (pp + pv * min_time > 0.0);
};


void base_object::add_pre(effector_p e)
{
  pre_attribs.add(e);
};


void base_object::add_post(effector_p e)
{
  post_attribs.add(e);
};

effector_list base_object::get_pre_attribs_copy()
{
  effector_list returnme;
  for (auto a: pre_attribs)
  {
    returnme.add(a.second->clone());
  };
  return returnme;
};

effector_list base_object::get_post_attribs_copy()
{
  effector_list returnme;
  for (auto a: post_attribs)
  {
    returnme.add(a.second->clone());
  };
  return returnme;
};

void base_object::drop_attrib(long long unsigned int i)
{
  dropped_attribs.push_back(i);
};


