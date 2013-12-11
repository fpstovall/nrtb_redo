/***********************************************
 This file is part of the NRTB project (https://*launchpad.net/nrtb).
 
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

#include "base_object.h"
#include <iostream>
#include <string>

using namespace nrtb;
using namespace std;

struct gravity : public abs_effector
{
  gravity()
  {
    handle = "gravity";
  };
  
  bool tick(base_object & o, int time)
  {
    o.accel_mod += triplet(9.82,0,0);
    return false;
  };
};

struct rocket : public abs_effector
{
  triplet impulse = triplet(1000.0,0.0,0.0);
  int burn_time = 3;
  
  bool tick(base_object & o, int time)
  {
    if (time <= burn_time)
      o.force += impulse;
    return false;
  };  
};

int main()
{
  bool failed = false;
  cout << "=========== sim messages test ============="
    << endl;


  cout << "=========== sim_messages test complete ============="
    << endl;
  
  return failed;
};



























