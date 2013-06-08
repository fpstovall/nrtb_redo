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


#include "obj_setup_info.h"
#include <iostream>

using namespace std;

int main()
{
  int returncode = 0;
  nrtb_com::triplet seeda(1,2,3);
  nrtb_com::triplet seedb = seeda + 1;
  nrtb_com::com_obj_setup t;
  t.uid = 1;
  t.name = "test_object";
  t.obj_type = 2;
  t.location.position.internal = seeda;
  t.location.position.internal = seedb;
  t.velocity.speed.internal = seedb;
  t.velocity.speed.internal = seeda;
  nrtb_msg::obj_setup_data gpb;
  t.load_message(&gpb);
  nrtb_com::com_obj_setup a(&gpb);
  if (!(t == a))
  {
	returncode = 1;
	cout << "nrtb_com::com_obj_setup unit test FAIL " << endl;
  }
  else
  {
	cout << "nrtb_com::com_obj_setup unit test PASS" << endl;
  };
	  
  return returncode;
};