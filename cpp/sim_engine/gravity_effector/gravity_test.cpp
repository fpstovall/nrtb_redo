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

#include "gravity.h"
#include <iostream>
#include <string>

using namespace nrtb;
using namespace std;

/*******************************
 * WARNING:
 * Don't do this at home kids. This method of recording
 * process information would be inheriently dangerious 
 * in the sim_core context. We can only use it here 
 * because we are ticking the object directly, something
 * which never happens in sim_engine context.
 * ******************************/
 
vector<triplet> locations;
vector<triplet> velocities;

// this is strictly designed as a post_attrib.
struct recorder : abs_effector
{
  string as_str() { return "recorder"; };
  effector_p clone()
  {
    return effector_p( new recorder(*this) );
  };
  bool tick(base_object & o, float quanta)
  {
    locations.push_back(o.location);
    velocities.push_back(o.velocity);
  };
};

struct faller : public base_object
{
  faller() 
    : base_object()
  {
    mass = 1;
    add_pre(effector_p(new norm_gravity));
    add_post(effector_p(new recorder));
    // put it high enough to fall for 10 seconds
    location = triplet(0.0,0.0,((9.80665*100)/2));
  };
  object_p clone()
  {
    faller * t = new faller(*this);
    t->pre_attribs = get_pre_attribs_copy();
    t->post_attribs = get_post_attribs_copy();
    return object_p(t);
  };
  bool apply_collision(object_p o,float duration) {return false;};
};

int main()
{
  bool failed = false;
  cout << "========== gravity test ============="
    << endl;

  faller icarus;
  float d = 1.0/50.0;

  cout << "Start from :" << icarus.location.z << endl;
  
  while (icarus.location.z > 0.0)
  {
    icarus.tick(d);
    icarus.apply(d);
  };
  
  cout << "Splat! : " << icarus.location.z << endl;
  
  float ticks = floor(locations.size());
    
  cout << "Icarus fell " << ticks/50.0 
    << " seconds." <<  endl;

  failed = failed or ((locations.size()/50.0) != 10.0);
  
  cout << "=========== gravity test complete ============="
    << endl;
  
  return failed;
};



























