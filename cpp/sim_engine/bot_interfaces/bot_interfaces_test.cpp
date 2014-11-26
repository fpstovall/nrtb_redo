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

#include "bot_interfaces.h"
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
    mass = 1;
  };
    
  bool apply_collision(object_p o) 
  {
    return true;
  };
/*
  bool tick(int quanta)
  {
    cout << quanta << "|" << id << ":" << radar.get_contacts() << endl;
    return base_object::tick(quanta);
  };
*/  
  base_object * clone()
  {
    my_object * returnme = new my_object(*this);
    returnme->pre_attribs = get_pre_attribs_copy();
    returnme->post_attribs = get_post_attribs_copy();
    return returnme;
  };
};

int main()
{
  bool failed = false;
  cout << "========== bot_interfaces test ============="
    << endl;
 
  
  if (failed)
    cout << " *** Unit test failed" << endl;
    
  cout << "=========== bot_interfaces test complete ============="
    << endl;
  
  return failed;
};



























