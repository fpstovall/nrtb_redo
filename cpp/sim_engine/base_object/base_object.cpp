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
  : axis(s) 
{};

void rotatable::trim()
{
  axis.x = fmodf(axis.x,period);
  axis.y = fmodf(axis.y,period);
  axis.z = fmodf(axis.z,period);
};

void rotatable::apply_force(float mass, float arm, triplet vec, float t)
{
  float I = (((arm*arm) * mass)/2)*t;
  axis.x += vec.x / I;
  axis.y += vec.y / I;
  axis.z += vec.z / I;
};

triplet rotatable::get_cos()
{
  triplet returnme;
  returnme.x = cosf(axis.x);
  returnme.y = cosf(axis.y);
  returnme.z = cosf(axis.z);
  return returnme;
};

triplet rotatable::get_sin()
{
  triplet returnme;
  returnme.x = sinf(axis.x);
  returnme.y = sinf(axis.y);
  returnme.z = sinf(axis.z);
  return returnme;
};

std::string base_object::as_str()
{
  std::stringstream returnme;
  returnme << "ID=" << id
    << ":loc=" << location
    << ":att=" << attitude.axis
    << ":vel=" << velocity
    << ":rot=" << rotation.axis
    << ":f=" << force
    << ":t=" << torque.axis
    << ":acc_mod=" << accel_mod
    << ":r_mod=" << rotation_mod.axis
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

bool base_object::tick(int time)
{
  // clean up for next pass
  accel_mod = 0;
  rotation_mod.axis = 0;
  force = 0;
  torque.axis = 0;
  mass_mod = 0;
  // execute any pending attrib drops
  for(auto i : dropped_attribs)
  {
    // ignore errors here
    try
    {
      pre_attribs.erase(i);
      post_attribs.erase(i);
    }
    catch (...) {};
  };
  dropped_attribs.clear();
  // execute the pre_attribs list.
  bool killme (false);
  for (auto e : pre_attribs)
    if (e.second->tick(*this, time))
      killme = true;
  return killme;
};

bool base_object::apply(int time, float quanta)
{
  // move acording to forces
  float tmass = mass + mass_mod;
  triplet a = force / tmass;
  triplet ev = velocity + (((a + accel_mod)/2.0) * quanta);
  velocity += (a  + accel_mod) * quanta;
  location += ev * quanta;
  // rotate according to the forces
  rotatable rbase = rotation;
  rotation.axis += rotation_mod.axis * quanta;
  rotation.apply_force(tmass, bounding_sphere.radius,
                       torque.axis,quanta);
  attitude.axis += (rotation.axis + rbase.axis)/2.0;
  attitude.trim();
  // apply post-effectors
  bool killme (false);
  for (auto e : post_attribs)
    if (e.second->tick(*this, time))
      killme = true;
  return killme;
};

bool base_object::check_collision(object_p o)
{
  float r = o->bounding_sphere.radius + bounding_sphere.radius;
  triplet adjusted = o->bounding_sphere.center;
  adjusted += o->location;
  return (r >= adjusted.range(bounding_sphere.center+location));
};

void base_object::add_pre(abs_effector* e)
{
  pre_attribs[e->id] = effector_p(e);
};

void base_object::add_pre(effector_p & e)
{
  pre_attribs[e->id] = e;
};


abs_effector& base_object::get_pre(long long unsigned int i)
{
  return *pre_attribs[i];
};

void base_object::add_post(abs_effector* e)
{
  post_attribs[e->id] = effector_p(e);
};

void base_object::add_post(effector_p & e)
{
  post_attribs[e->id] = e;
};

abs_effector& base_object::get_post(long long unsigned int i)
{
  return *post_attribs[i];
};

effector_list base_object::get_pre_attribs_copy()
{
  effector_list returnme;
  for (auto a: pre_attribs)
  {
    returnme[a.first] = effector_p(a.second->clone());
  };
  return returnme;
};

effector_list base_object::get_post_attribs_copy()
{
  effector_list returnme;
  for (auto a: post_attribs)
  {
    returnme[a.first] = effector_p(a.second->clone());
  };
  return returnme;
};

void base_object::drop_attrib(long long unsigned int i)
{
  dropped_attribs.push_back(i);
};


