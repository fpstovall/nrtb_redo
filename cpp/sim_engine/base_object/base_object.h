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
 
#ifndef base_object_header
#define base_object_header

#include <triad.h>
#include <serializer.h>
#include <map>
#include <memory>

namespace nrtb
{

const float pi=3.14159;
  
typedef triad<float> triplet;

struct rotatable
{
  rotatable(triplet s=triplet(0.0));
  triplet axis;
  const float period = 2.0 * pi;
  void trim();
  void apply_force(float mass, float arm, triplet vec, float t);
  triplet get_cos();
  triplet get_sin();
};

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
  virtual ~abs_effector() {};
  // polymorphic copier
  virtual abs_effector * clone() = 0;
  unsigned long long id = effector_num();
  std::string handle;
  virtual std::string as_str() = 0;
  virtual bool tick(base_object & o, int time) = 0;
};
  
typedef std::shared_ptr<abs_effector> effector_p;
typedef std::map<unsigned long long, effector_p> effector_list;

struct base_object
{
  static serializer object_num;
  virtual ~base_object() {};
  // polymophic copier
  virtual base_object * clone() = 0;
  // data
  unsigned long long id = object_num();
  std::string handle;
  triplet location;
  rotatable attitude;
  triplet velocity;
  rotatable rotation;
  triplet force;
  rotatable torque;
  triplet accel_mod;
  rotatable torque_mod;
  float mass;
  float mass_mod;
  sphere bounding_sphere;
  // reporting
  virtual std::string as_str();
  // effector management
  virtual void add_pre(abs_effector * e);
  virtual void add_pre(effector_p & e);
  virtual abs_effector & get_pre(unsigned long long i);
  virtual void add_post(abs_effector * e);
  virtual void add_post(effector_p & e);
  virtual abs_effector & get_post(unsigned long long i);
  virtual void drop_attrib(unsigned long long i);
  // -- these return deep copies of the attribs list.
  virtual effector_list get_pre_attribs_copy();
  virtual effector_list get_post_attribs_copy();
  // sim methods
  // returns true if a collision is detected.
  virtual bool check_collision(object_p o);
  // the following return true if the object is destroyed.
  virtual bool tick(int time);
  virtual bool apply(int time, float quanta);
  virtual bool apply_collision(object_p o) = 0;
protected:
  effector_list pre_attribs;
  effector_list post_attribs; 
  std::vector<unsigned long long> dropped_attribs;
};

} // namepace nrtb

#endif // base_object_header
