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

#include "railgun.h"
#include <base_object.h>
#include <iostream>
#include <string>

using namespace nrtb;
using namespace std;

struct my_object : public base_object
{  
  my_object()
  {
    location = triplet(0,0,0);
    velocity = triplet(0,0,0);
    bounding_sphere.center = triplet(0,0,0);
    bounding_sphere.radius = 1;
    add_pre(std::make_shared<radar_mk1>());
    mass = 1;
    object_type = 2;
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

int main()
{
  bool failed = false;
  cout << "========== radar_mk1 test ============="
    << endl;
 
  
  // start a sim_core;
  sim_core & w = global_sim_core::get_reference();
  w.start_sim();
  chrono::milliseconds nap(10);
  //while (!w.running()) this_thread::sleep_for(nap); 
  
  
  if (failed)
    cout << " *** Unit test failed" << endl;
    
  cout << "=========== radar_mk1 test complete ============="
    << endl;
  
  return failed;
};
