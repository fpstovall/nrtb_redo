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
#include "bcp_server.h"
#include <iostream>
#include <string>
#include <future>

using namespace nrtb;
using namespace std;

int main()
{
  bool failed = false;
  cout << "========== bcp_server test ============="
    << endl;

  sim_core sc1(1.0/50.0);
  sc1.start_sim();
  bcp_listener listener(sc1);
  listener.start();
  chrono::milliseconds t(2500);
  this_thread::sleep_for(t);
  listener.stop();
  sc1.stop_sim();
  
    
  cout << "=========== bcp_server test complete ============="
    << endl;

failed = true;    
    
  return failed;
};



























