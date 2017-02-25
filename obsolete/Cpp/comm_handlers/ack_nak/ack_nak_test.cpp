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


#include "ack_nak.h"
#include <iostream>

using namespace std;

int main()
{
  int returncode = 0;
  {
	nrtb_com::com_ack t;
	t.msg_uid = 2;
	nrtb_msg::message_ack gpb;
	t.load_message(&gpb);
	nrtb_com::com_ack a(&gpb);
	if (!(t == a))
	{
	  returncode = 1;
	  cout << "nrtb_com::com_ack unit test FAIL " << endl;
	}
	else
	{
	  cout << "nrtb_com::com_ack unit test PASS" << endl;
	};
  }
  {
	nrtb_com::com_nak t;
	t.reason = nrtb_com::com_nak::sequence_error;
	t.msg_uid = 2;
	t.last_good_msg_uid = 3;
	nrtb_msg::message_nak gpb;
	t.load_message(&gpb);
	nrtb_com::com_nak a(&gpb);
	if (!(t == a))
	{
	  returncode = 1;
	  cout << "nrtb_com::com_nak unit test FAIL " << endl;
	}
	else
	{
	  cout << "nrtb_com::com_nak unit test PASS" << endl;
	};
  }
	  
  return returncode;
};