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

namespace nrtb
{
  
class base_object;

typedef triad<float> triplet;

struct sphere
{
  triplet center;
  float radius;
};

class abs_adjustor
{
public:
  virtual bool tick(base_object & o, int time) = 0;
};
  
typedef list<abs_adjustor> adjuster_list;

class abs_object
{
public:
  // data
  triplet location;
  triplet attitude;
  triplet velocity;
  triplet rotation;
  triplet force;
  triplet torque;
  float mass;
  sphere bounding_sphere;
  adjuster_list pre_attribs;
  adjuster_list post_attribs;
  list<abs_object> components;
  // methods
  void tick(int time);
  void apply(int time);
  bool check_collision(sphere s);
};

} // namepace nrtb

#endif // base_object_header
