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


#include "sim_termination.h"
#include <iostream>

using namespace std;

int main()
{
  int returncode = 0;
  nrtb_com::com_sim_termination t;
  // setup single vars.
  t.last_quanta = 8126313;
  t.reason = "test message";
  t.error_code = 12;
  t.avg_cook_ms = 17.342;
  t.avg_quanta_ms = 1000/50;
  // run the test.
  nrtb_msg::sim_termination_data gpb;
  t.load_message(&gpb);
  nrtb_com::com_sim_termination a(&gpb);
  if (!(t == a))
  {
	returncode = 1;
	cout << "nrtb_com::com_sim_termination unit test FAIL " << endl;
  }
  else
  {
	cout << "nrtb_com::com_sim_termination unit test PASS" << endl;
  };
	  
  return returncode;
};