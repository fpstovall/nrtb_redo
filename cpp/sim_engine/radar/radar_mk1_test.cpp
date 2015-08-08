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

#include "radar_mk1.h"
#include <base_object.h>
#include <iostream>
#include <string>

using namespace nrtb;
using namespace std;

struct my_object : public base_object
{  
  radar_mk1 radar;
  
  my_object() : radar(radar_mk1(*this))
  {
    location = triplet(0,0,0);
    velocity = triplet(0,0,0);
    bounding_sphere.center = triplet(0,0,0);
    bounding_sphere.radius = 1;
    mass = 1;
  };
  
  bool apply_collision(object_p o, float duration) 
  {
    return true;
  };
  
  object_p clone()
  {
    my_object * returnme = new my_object(*this);
    returnme->pre_attribs = get_pre_attribs_copy();
    returnme->post_attribs = get_post_attribs_copy();
    return object_p(returnme);
  };
};

typedef shared_ptr<my_object> my_object_p;

typedef vector<sensor_rec> my_contacts;

my_contacts parse_contacts(string s)
{
  my_contacts returnme;
  stringstream in(s);
  int count;
  in >> count;
  int worked = 0;
  while (worked < count)
  {
    sensor_rec c;
    in >> c.type;
    in >> c.location;
    in >> c.velocity;
    returnme.push_back(c);
    worked++;
  };
  return returnme;
};

int main()
{
  bool failed = false;
  cout << "========== radar_mk1 test ============="
    << endl;
 
  // get a couple target objects.
  my_object_p o1(new my_object);
  my_object_p o2(new my_object);
  o2->location = triplet(0,5,5);
  my_object_p o3(new my_object);
  o3->location = triplet(0,-5,5);
  o3->attitude.set(triplet(0,0,pi/2.0));
  o3->velocity = triplet(1.0,0,0);
    
  // start a sim_core;
  sim_core & w = global_sim_core::get_reference();
  w.start_sim();
  chrono::milliseconds nap(10);
  //while (!w.running()) this_thread::sleep_for(nap); 
  
  // Insert objects in the sim.
  w.add_object(object_p(o1));
  w.add_object(object_p(o2));
  w.add_object(object_p(o3));
  chrono::milliseconds pause(50);
  this_thread::sleep_for(pause);
  
  // Verify they see each other properly.
  string o1c, o2c, o3c;
  o1->radar.command("radar contacts",o1c);
  o2->radar.command("radar contacts",o2c);
  o3->radar.command("radar contacts",o3c);
  
/*  
  string o1c = o1->radar.get_contacts();
  string o2c = o2->radar.get_contacts();  
  string o3c = o3->radar.get_contacts();  
*/

  w.stop_sim();

  cout << "from o1: " << o1c << endl;
  cout << "from o2: " << o2c << endl;
  cout << "from o3: " << o3c << endl;

  my_contacts o1l = parse_contacts(o1c);
  my_contacts o2l = parse_contacts(o2c);
  my_contacts o3l = parse_contacts(o3c);

  failed = (o1l.size() !=2) or (o2l.size() != 2) or (o3l.size() != 2);  
  bool o1t = (o1l[0].location != triplet(7.07107,1.5708,0.785398))
    or (o1l[0].velocity != triplet(0,0,0))
    or (o1l[1].location != triplet(7.07132,-1.5588,0.785434))
    or (o1l[1].velocity != triplet(1,0,0));

  if (o1t)
    cout << "  ** " << o1l[0].location << o1l[0].velocity 
      << o1l[1].location << o1l[1].velocity << endl;

//  failed = 
//    (o1c != "2 1 7.07107 1.5708 0.785398 (0,0,0) 1 7.07107 -1.5708 2.35619 (0,0,0)")
//    or 
//    (o2c != "2 1 7.07107 -1.5708 -2.35619 (0,0,0) 1 10 -1.5708 3.14159 (0,0,0)")
//    or 
//    (o3c != "2 1 7.07107 1.3113e-06 -0.785398 (0,0,0) 1 10 1.3113e-06 0 (0,0,0)");


  failed = failed or o1t;
  
  if (failed)
    cout << " *** Unit test failed" << endl;
    
  cout << "=========== radar_mk1 test complete ============="
    << endl;
  
  return failed;
};



























