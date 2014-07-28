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


#include "tq_obj.h"
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
  int returncode = 0;
  nrtb_com::triplet seeda(1,2,3);
  nrtb_com::triplet seedb = seeda + 1;
  nrtb_com::com_tq_obj_update t;
  t.obj_uid = 1;
  t.location.position.internal = seeda;
  t.location.position.internal = seedb;
  t.velocity.speed.internal = seedb;
  t.velocity.speed.internal = seeda;
  // populate events
  for (int i=0; i<10; i++)
  {
	stringstream buffer;
	buffer << "event " << i;
	t.event.push_back(buffer.str());
  };
  // populate inbound_com
  for (int i=0; i<10; i++)
  {
	stringstream buffer;
	buffer << "Command " << i;
	t.inbound_com.push_back(buffer.str());
  };
  // populate outbound_com
  for (int i=0; i<10; i++)
  {
	stringstream buffer;
	buffer << "Response " << i;
	t.outbound_com.push_back(buffer.str());
  };
  // start test.
  nrtb_msg::sim_obj_tq_data gpb;
  t.load_message(&gpb);
  nrtb_com::com_tq_obj_update a(&gpb);
  if (!(t == a))
  {
	returncode = 1;
	cout << "nrtb_com::com_tq_obj_update unit test FAIL " << endl;
  }
  else
  {
	cout << "nrtb_com::com_tq_obj_update unit test PASS" << endl;
  };
	  
  return returncode;
};