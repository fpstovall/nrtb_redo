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

#include "hover.h"
#include <gravity.h>
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
  abs_effector * clone()
  {
    return new recorder(*this);
  };
  bool tick(base_object & o, int time)
  {
    locations.push_back(o.location);
    velocities.push_back(o.velocity);
  };
};

struct floater : public base_object
{
  floater()
  {
    add_pre(new hover(1.0,0.90,1.0));
    add_pre(new norm_gravity);
    add_post(new recorder);
    // start it at the set height.
    location = triplet(0.0,0.0,1.0);
    velocity = 0.0;
  };
  base_object * clone()
  {
    floater * t = new floater(*this);
    t->pre_attribs = get_pre_attribs_copy();
    t->post_attribs = get_post_attribs_copy();
    return t;
  };
  bool apply_collision(object_p o) {return false;};
};

int main()
{
  bool failed = false;
  cout << "========== ground_effect test ============="
    << endl;

  int counter = 0;
  floater zepplin;
  while (counter < 50)
  {
    counter++;
    zepplin.tick(counter);
    zepplin.apply(counter,1/50.0);
  };
  
  cout << "hovered at " << zepplin.location.z << " meters." << endl;

  velocities.clear();
  locations.clear();
  zepplin.location.z = 0.95;
  counter = 0;
  while (counter < 1000)
  {
    counter++;
    zepplin.tick(counter);
    zepplin.apply(counter,1/50.0);
  };
  
  cout << "hovered at " << zepplin.location.z << " meters." << endl;
  
  for(int i=950; i<1000; i++)
    cout << i << ":" << locations[i] << velocities[i] << endl;
  
  cout << "=========== ground_effect test complete ============="
    << endl;
  
  return failed;
};



























