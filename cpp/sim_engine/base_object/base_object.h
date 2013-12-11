/***********************************************
 This file is part of the NRTB project (https://*launchpad.net/nrtb).
 
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
 
#ifndef base_object_header
#define base_object_header

#include <triad.h>
#include <serializer.h>
#include <map>
#include <memory>

namespace nrtb
{
  
typedef triad<float> triplet;

struct sphere
{
  triplet center;
  float radius;
};

struct base_object;
typedef std::shared_ptr<base_object> object_p;
typedef std::map<unsigned long long, object_p> object_list;

struct abs_effector
{
  static serializer effector_num;
  unsigned long long id = effector_num();
  std::string handle;
  virtual bool tick(base_object & o, int time) = 0;
};
  
typedef std::shared_ptr<abs_effector> effector_p;
typedef std::map<unsigned long long, effector_p> effector_list;

struct base_object
{
  static serializer object_num;
  // data
  unsigned long long id = object_num();
  std::string handle;
  triplet location;
  triplet attitude;
  triplet velocity;
  triplet rotation;
  triplet force;
  triplet torque;
  triplet accel_mod;
  triplet torque_mod;
  float mass;
  float mass_mod;
  sphere bounding_sphere;
  effector_list pre_attribs;
  effector_list post_attribs;
  // sim methods
  virtual bool tick(int time);
  virtual bool apply(int time, float quanta);
  virtual bool check_collision(sphere s);
};

} // namepace nrtb

#endif // base_object_header
