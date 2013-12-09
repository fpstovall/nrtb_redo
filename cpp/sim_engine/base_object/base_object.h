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
#include <list>
#include <memory.h>

namespace nrtb
{
  
typedef triad<float> triplet;

struct sphere
{
  triplet center;
  float radius;
};

struct base_object;
typedef std::unique_ptr<base_object> object_p;
typedef std::list<object_p> object_list;

struct abs_adjustor
{
  std::string handle;
  virtual bool tick(base_object & o, int time) = 0;
};
  
typedef std::shared_ptr<abs_adjustor> adjustor_p;
typedef std::list<adjustor_p> adjustor_list;

struct base_object
{
  // data
  std::string handle;
  triplet location;
  triplet attitude;
  triplet velocity;
  triplet rotation;
  triplet force;
  triplet torque;
  float mass;
  sphere bounding_sphere;
  adjustor_list pre_attribs;
  adjustor_list post_attribs;
  object_list components;
  // sim methods
  virtual void tick(int time);
  virtual void apply(int time);
  virtual bool check_collision(sphere s);
};

} // namepace nrtb

#endif // base_object_header
