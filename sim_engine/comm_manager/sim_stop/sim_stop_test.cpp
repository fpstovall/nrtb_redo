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


#include "sim_stop.h"
#include <iostream>

using namespace std;

int main()
{
  int returncode = 0;
  nrtb_com::com_request_sim_stop t;
  // setup single vars.
  t.failed = true;
  t.reason = "test message";
  // run the test.
  nrtb_msg::db_request_sim_stop gpb;
  t.load_message(&gpb);
  nrtb_com::com_request_sim_stop a(&gpb);
  if (!(t == a))
  {
	returncode = 1;
	cout << "nrtb_com::com_request_sim_stop unit test FAIL " << endl;
  }
  else
  {
	cout << "nrtb_com::com_request_sim_stop unit test PASS" << endl;
  };
	  
  return returncode;
};