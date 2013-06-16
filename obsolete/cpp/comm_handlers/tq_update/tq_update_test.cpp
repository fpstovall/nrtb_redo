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


#include "tq_update.h"
#include <serializer.h>
#include <iostream>
#include <sstream>

using namespace std;

nrtb::serializer sequence;

nrtb_com::com_tq_obj_update mk_update()
{
  nrtb_com::com_tq_obj_update returnme;
  returnme.obj_uid = sequence();
  returnme.location.position.internal = sequence();
  returnme.location.attitude.internal = sequence();
  returnme.velocity.speed.internal = sequence();
  returnme.velocity.rotation.internal = sequence();
  // load events
  for (int i=0; i<10; i++)
  {
	stringstream buffer;
	buffer << "Event " << sequence();
	returnme.event.push_back(buffer.str());
  };
  // load inbound
  for (int i=0; i<10; i++)
  {
	stringstream buffer;
	buffer << "Inbound " << sequence();
	returnme.inbound_com.push_back(buffer.str());
  };
  // load outbound
  for (int i=0; i<10; i++)
  {
	stringstream buffer;
	buffer << "Outbound " << sequence();
	returnme.outbound_com.push_back(buffer.str());
  };
  return returnme;
};

nrtb_com::com_obj_setup mk_new()
{
  nrtb_com::com_obj_setup returnme;
  returnme.uid = sequence();
  stringstream buffer;
  buffer << "Object " << sequence();
  returnme.name = buffer.str();
  returnme.location.position.internal = sequence();
  returnme.location.attitude.internal = sequence();
  returnme.velocity.speed.internal = sequence();
  returnme.velocity.rotation.internal = sequence();
  returnme.obj_type = sequence();
  return returnme;
};

int main()
{
  int returncode = 0;
  nrtb_com::triplet seeda(1,2,3);
  nrtb_com::triplet seedb = seeda + 1;
  nrtb_com::com_tq_update t;
  for (int i=0; i<45; i++)
  {
	t.obj_data.push_back(mk_update());
  };
  for (int i=0; i<5; i++)
  {
	t.new_objs.push_back(mk_new());
  };
  // start test.
  nrtb_msg::tq_data gpb;
  t.load_message(&gpb);
  nrtb_com::com_tq_update a(&gpb);
  if (!(t == a))
  {
	returncode = 1;
	cout << "nrtb_com::com_tq_update unit test FAIL " << endl;
  }
  else
  {
	cout << "nrtb_com::com_tq_update unit test PASS" << endl;
  };
	  
  return returncode;
};