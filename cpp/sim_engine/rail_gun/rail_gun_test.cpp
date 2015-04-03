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

#include <sim_core.h>
#include "rail_gun.h"
#include <base_object.h>
#include <bot_interfaces.h>
#include <iostream>
#include <string>

using namespace nrtb;
using namespace std;

struct my_object : public abs_bot
{
  shared_ptr<rail_gun_mk1> cannon;
  
  my_object()
  {
    location = triplet(0,0,0);
    velocity = triplet(0,0,0);
    bounding_sphere.center = triplet(0,0,0);
    bounding_sphere.radius = 1;
    cannon.reset(new rail_gun_mk1(*this));
    mass = 1;
  };
  
  
  bool apply_collision(object_p o, float quanta) 
  {
    return true;
  };

  base_object * clone()
  {
    my_object * returnme = new my_object(*this);
    returnme->pre_attribs = get_pre_attribs_copy();
    returnme->post_attribs = get_post_attribs_copy();
    return returnme;
  };
  
  void bot_cmd(string cmd) 
  {
    cout << "cmd: " << cmd << endl;
  };
  
  void send_to_bcp(string msg) 
  {
    cout << "stb: " << msg << endl;
  };
};


int main()
{
  bool failed = false;
  cout << "========== rail_gun test ============="
    << endl;
 
  // create sim engine
  sim_core & sim = global_sim_core::get_reference();
    
  // Create test bot.
  object_p b1(new my_object);
    
  // test goal seek
    
  
  // test unconditional fire.
    
  // test fire on stable.
    
  // kill another object
    

    
  cout << "=========== rail_gun test complete ============="
    << endl;
  
  return failed;
};



























