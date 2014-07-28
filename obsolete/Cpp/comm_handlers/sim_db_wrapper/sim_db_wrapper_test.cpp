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


#include "sim_db_wrapper.h"
#include <iostream>

using namespace std;

int main()
{
  int returncode = 0;
  nrtb_com::com_sim_to_db t;
  t.msg_uid = 2;
  t.ack.reset(new nrtb_com::com_ack);
  t.nak.reset(new nrtb_com::com_nak);
  t.quanta_results.reset(new nrtb_com::com_tq_update);
  t.sim_setup.reset(new nrtb_com::com_sim_setup);
  t.sim_end.reset(new nrtb_com::com_sim_termination);
  t.db_abort.reset(new nrtb_com::com_request_sim_stop);
  nrtb_msg::sim_to_db gpb;
  t.load_message(&gpb);
  nrtb_com::com_sim_to_db a(&gpb);
  if (!(t == a))
  {
	returncode = 1;
	cout << "nrtb_com::com_sim_to_db unit test FAIL " << endl;
  }
  else
  {
	cout << "nrtb_com::com_sim_to_db unit test PASS" << endl;
  };
	  
  return returncode;
};