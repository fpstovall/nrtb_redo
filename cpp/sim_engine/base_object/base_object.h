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

// 3d vector used throughout NRTB
typedef triad<float> triplet;

/***************************************************************
 * rotatable is sort of a specialized triplet designed to 
 * handled rotation values, as in rate of rotate or attitude
 * All input values are radians unless otherwise noted.
 **************************************************************/
struct rotatable
{
  // Construct from a triplet. All values are assumed to be radians.
  rotatable(triplet s=triplet(0.0));
  // copy constructor.
  rotatable(rotatable& a);
  // convienence const.. one full circle.
  const float period = 2.0 * pi;
  // mods all values to 0 <= n < period.
  void trim();
  /**** apply_force is used to alter rotation by linear force.
   * mass is the mass of the impacting object.
   * arm is the distance from the impact to the center of rotation
   * vec is the cartesian motion vector of the impactor.
   * t is the time in sections the force is applied. 
   */
  void apply_force(float mass, float arm, triplet vec, float t);
  // scale the rotatable by the triplet.
  void scale(triplet factor);
  // add the triplet to the rotatable.
  void add(triplet value);
  // Set the rotatable's values to match the triplet.
  // Values are assumed to be radians.
  void set(triplet a);
  // Set this rotatable to the one provided.
  void set(rotatable a);
  // returns a triplet with 3 axis rotations.
  triplet angles();
  // returns the cosines of all 3 axis.
  triplet get_cos();
  // returns the signs of all 3 axis.
  triplet get_sin();
private:
  std::atomic<bool> dirty;
  triplet axis;
  triplet cos;
  triplet sin;
  void recalc();
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
  // provides a unique id for each effector.
  static serializer effector_num;
  // Populated from the serializer at allocation.
  unsigned long long id = effector_num();
  // NOP destructor to ensure proper deallocation.
  virtual ~abs_effector() {};
  /** polymorphic copier
  /* Returns a newly allocated copy of the effector.
   * Must be overridden by each child class.
   */
  virtual std::shared_ptr<abs_effector> clone() = 0;
  // name of the effector.
  std::string handle;
  /** Reporting method
   * Overridden by each descendent, this method 
   * should return a string capturing the effector's 
   * status at the time of the call.
   */
  virtual std::string as_str() = 0;
  /** does the "time quanta" work of the effector.
   * Returns true if the object should die.
   * o is the parent object
   * quanta is the duration of this time-slice.
   */
  virtual bool tick(base_object & o, float quanta) = 0;
};
  
typedef std::shared_ptr<abs_effector> effector_p;
//typedef std::map<unsigned long long, effector_p> effector_list;

class effector_list : public std::map<unsigned long long, effector_p>
{
public:
  void add(effector_p e);
  void remove(effector_p e);
  void remove(unsigned long long key);
};

struct base_object
{
  static serializer object_num;
  virtual ~base_object() {};
  // polymophic copier
  virtual object_p clone() = 0;
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
  rotatable rotation_mod;
  float mass;
  float mass_mod;
  sphere bounding_sphere;
  // reporting
  virtual std::string as_str();
  // effector management
  virtual void add_pre(effector_p e);
  virtual void add_post(effector_p e);
  virtual void drop_attrib(unsigned long long i);
  // -- these return deep copies of the attribs list.
  virtual effector_list get_pre_attribs_copy();
  virtual effector_list get_post_attribs_copy();
  // sim methods
  // returns true if a collision is detected.
  virtual bool check_collision(object_p o, float quanta);
  // the following return true if the object is destroyed.
  virtual bool tick(float quanta);
  virtual bool apply(float quanta);
  virtual bool apply_collision(object_p o, float quanta) = 0;
protected:
  effector_list pre_attribs;
  effector_list post_attribs; 
  std::vector<unsigned long long> dropped_attribs;
};

} // namepace nrtb

#endif // base_object_header
