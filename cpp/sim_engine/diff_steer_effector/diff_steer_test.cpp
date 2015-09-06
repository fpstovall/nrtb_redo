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

#include "diff_steer.h"
#include <gravity.h>
#include <hover.h>
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

struct driver : public base_object
{
  // diff_steer interface
  shared_ptr<diff_steer>  mobility;
  // constructor
  driver()
  {
    mobility.reset(new diff_steer(*this,100,1000,pi,10,8));
    add_pre(effector_p(new norm_gravity));
    add_pre(effector_p(new hover(1.25,0.10,2.0)));
    add_post(effector_p(new recorder));
    mass=100.0;
    bounding_sphere.radius = 1.0;
    location.z = 1.25;
  };
  // clone method
  object_p clone()
  {
    driver * t = new driver(*this);
    t->pre_attribs = get_pre_attribs_copy();
    t->post_attribs = get_post_attribs_copy();
    return object_p(t);
  };
  // nop collision application.
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
  cout << "========== diff_steer test ============="
    << endl;

  try
  {
    cout << "Initial state test: ";
    driver test_ob;
    triplet startL = test_ob.location;
    float d = 1.0/50.0;
    std:;string response;
    for(int i=0; i<50; i++)
    {
      test_ob.tick(d);
      test_ob.apply(d);
    };
    bool test_failed = (test_ob.location.x != 0.0)
                    or (test_ob.location.y != 0.0)
                    or (test_ob.velocity.x != 0.0)
                    or (test_ob.velocity.y != 0.0);
    failed = failed or test_failed;
    cout << (test_failed ? "Failed" : "Passed") << " " 
      << test_ob.location << test_ob.velocity << endl;
      
    cout << "Simple drive test: ";
    test_ob.mobility->command("drive power 100", response);
    test_ob.mobility->command("drive brake 0", response);
    bool done = false;
    int i = 0;
    while (!done)
    {
      test_ob.tick(d);
      test_ob.apply(d);
      i++;
      done = (test_ob.location.x >= 1.0) or (i > 80);
    };
    test_failed = (test_ob.location.y != 0.0)
      or (i != 71)
      or (test_ob.velocity.y != 0.0)
      or (fabsf(test_ob.velocity.x - 1.42) > 1e-4);
    failed = failed or test_failed;
    cout << (test_failed ? "Failed" : "Passed") << " " << i << " "
      << test_ob.location << test_ob.velocity << endl;

    cout << "Simple braking test: ";
    test_ob.mobility->command("drive power 0", response);
    test_ob.mobility->command("drive brake 100", response);
    i=0;
    while ((test_ob.velocity.x > 0.0) or (test_ob.velocity.y > 0.0))
    {
      test_ob.tick(d);
      test_ob.apply(d);
      i++;
      if (i > 29) break;
    }; 
    test_failed = (i != 8);
    failed = failed or test_failed;
    cout << (test_failed ? "Failed" : "Passed") << " "
      << i << " " << test_ob.location << test_ob.velocity << endl; 

    cout << "Simple turning test: ";
    test_ob.mobility->command("drive power 0.0", response);
    test_ob.mobility->command("drive brake 100.0", response);
    test_ob.mobility->command("drive turn 100.0", response);
    i=0;
    // rotate 90 degrees in 1/2 second.
    while (test_ob.attitude.angles().z < (pi/2.0))
    {
      test_ob.tick(d);
      test_ob.apply(d);
      i++;
      if (i > 49) break;
    }; 
    test_failed = (i != 26);
    failed = failed or test_failed;
    cout << (test_failed ? "Failed" : "Passed") << " "
      << i << " " << test_ob.attitude.angles() << endl; 

    cout << "Compound movement test: ";
    test_ob.mobility->command("drive power 100.0", response);
    test_ob.mobility->command("drive brake 0.0", response);
    test_ob.mobility->command("drive turn -10.0", response);
    i=0;
    // rotate 90 degrees in 5 second.
    while (test_ob.attitude.angles().z > 0.0)
    {
      test_ob.tick(d);
      test_ob.apply(d);
      i++;
      if (i > 300) break;
    }; 
    test_failed = (i != 261);
    failed = failed or test_failed;
    cout << (test_failed ? "Failed" : "Passed") << " "
      << i << " " << test_ob.attitude.angles() 
      << test_ob.location << test_ob.velocity << endl; 

    // Status command test.
    cout << "Status command test: ";
    bool handled = test_ob.mobility->command("drive status", response);
    test_failed = (!handled) 
      or (response != "drive status 1 0 -0.1");
    failed = failed or test_failed;
    cout << (test_failed ? "Failed" : "Passed") << " ("
      << response << ")" << endl; 
      
    // Bad command test
    cout << "Invalid command test: ";
    handled = test_ob.mobility->command("drive fubar", response);
    test_failed = (!handled) 
      or (response != "bad_cmd \"drive fubar\"");
    failed = failed or test_failed;
    cout << (test_failed ? "Failed" : "Passed") << " ("
      << response << ")" << endl; 
      
    // unhandled command test
    cout << "Unhandled command test: ";
    handled = test_ob.mobility->command("nota command", response);
    test_failed = (handled); 
    failed = failed or test_failed;
    cout << (test_failed ? "Failed" : "Passed") << " ("
      << response << ")" << endl; 
    
  }
  catch (base_exception &e)
  {
    failed = true;
    cout << e.comment() << endl;
  }
  catch (std::exception &e)
  {
    failed = true;
    cout << "Exception: " << e.what() << endl;
  }
  catch (...)
  {
    failed = true;
    cout << "GACK!" << endl;    
  };
  cout << "=========== diff_steer test complete ============="
    << endl;
  
  return failed;
};



























