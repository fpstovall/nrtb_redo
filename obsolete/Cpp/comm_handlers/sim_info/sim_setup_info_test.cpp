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


#include "sim_setup_info.h"
#include <iostream>

using namespace std;

int main()
{
  int returncode = 0;
  nrtb_com::com_sim_setup t;
  // setup single vars.
  t.name = "test sim";
  t.model = "test model";
  t.quanta_ms = 1000 / 50;
  t.max_quantas = 100;
  t.started_by = "me";
  // setup 50 objects
  for (int i=0; i<50; i++)
  {
	nrtb_com::com_sim_setup::obj_p o(new nrtb_com::com_obj_setup());
	o->uid = i;
	t.items.push_back(std::move(o));
  };
  // run the test.
  nrtb_msg::sim_setup_data gpb;
  t.load_message(&gpb);
  nrtb_com::com_sim_setup a(&gpb);
  if (!(t == a))
  {
	returncode = 1;
	cout << "nrtb_com::com_sim_setup unit test FAIL " << endl;
  }
  else
  {
	cout << "nrtb_com::com_sim_setup unit test PASS" << endl;
  };
	  
  return returncode;
};