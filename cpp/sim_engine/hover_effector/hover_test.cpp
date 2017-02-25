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
  effector_p clone()
  {
    return effector_p(new recorder(*this));
  };
  bool tick(base_object & o, float quanta)
  {
    locations.push_back(o.location);
    velocities.push_back(o.velocity);
  };
};

struct floater : public base_object
{
  floater()
  {
    add_pre(effector_p(new hover(1.0,0.5,2.0)));
    add_pre(effector_p(new norm_gravity));
    add_post(effector_p(new recorder));
    // start it at the set height.
    mass=1.0;
    location = triplet(0.0,0.0,1.0);
    velocity = 0.0;
  };
  object_p clone()
  {
    floater * t = new floater(*this);
    t->pre_attribs = get_pre_attribs_copy();
    t->post_attribs = get_post_attribs_copy();
    return object_p(t);
  };
  bool apply_collision(object_p o, float duration) {return false;};
};

typedef map<std::string,double> rtype;
rtype report(vector<triplet> v)
{
  double min = 1e6;
  double max = 0.0;
  double avg = 0.0;
  double count = 0;
  for(auto t: v)
  {
    count++;
    min = (min < t.z) ? min : t.z;
    max = (max > t.z) ? max : t.z;
    avg += t.z;
  };
  avg /= count;
  rtype returnme;
  returnme["min"] = min;
  returnme["max"] = max;
  returnme["avg"] = avg;
  return returnme;
};

string write_details(rtype l, rtype v)
{
  stringstream s;
  s << "Alt : " << l["min"] << ":" 
    << l["avg"] << ":" << l["max"] 
    << "\nVel : " << v["min"] << ":" 
    << v["avg"] << ":" << v["max"];
  return s.str();
};

int main()
{
  bool failed = false;
  cout << "========== ground_effect test ============="
    << endl;

  int counter = 0;
  float d = 1.0/50.0;
  floater zepplin;
  while (counter < 500)
  {
    counter++;
    zepplin.tick(d);
    zepplin.apply(d);
    if (counter == 449)
    {
      locations.clear();
      velocities.clear();
    };
  };

  auto a = report(locations);
  auto v = report(velocities);
  cout << "Start from set:\n" << write_details(a,v) << endl;
  failed = failed or (fabs(a["avg"]-1.0) > 0.01);
  failed = failed or (fabs(a["min"]-1.0) > 0.02);
  failed = failed or (fabs(a["max"]-1.0) > 0.02);
  
  velocities.clear();
  locations.clear();
  zepplin.velocity.z = 0.0;
  zepplin.location.z = 0.0;
  counter = 0;
  while (counter < 500)
  {
    counter++;
    zepplin.tick(d);
    zepplin.apply(d);
    if (counter == 449)
    {
      locations.clear();
      velocities.clear();
    };
  };
  
  a = report(locations);
  v = report(velocities);
  cout << "Start from below:\n" << write_details(a,v) << endl;
  failed = failed or (fabs(a["avg"]-1.0) > 0.01);
  failed = failed or (fabs(a["min"]-1.0) > 0.04);
  failed = failed or (fabs(a["max"]-1.0) > 0.04);
  
  velocities.clear();
  locations.clear();
  zepplin.velocity.z = 0.0;
  zepplin.location.z = 2.0;
  counter = 0;
  while (counter < 500)
  {
    counter++;
    zepplin.tick(d);
    zepplin.apply(d);
    if (counter == 449)
    {
      locations.clear();
      velocities.clear();
    };
  };
  
  a = report(locations);
  v = report(velocities);
  cout << "Start from above:\n" << write_details(a,v) << endl;
  failed = failed or (fabs(a["avg"]-1.0) > 0.01);
  failed = failed or (fabs(a["min"]-1.0) > 0.04);
  failed = failed or (fabs(a["max"]-1.0) > 0.04);
  
  
  cout << "=========== ground_effect test complete ============="
    << endl;
  
  return failed;
};



























