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

tcp_socket_p new_conn()
{
  tcp_socket_p sock(new tcp_socket);
  sock->connect("localhost:64500");
  return std::move(sock);
};

int main()
{
  bool failed = false;
  cout << "========== bcp_server test ============="
    << endl;

  // use the listener to make some bots.
  sim_core sc1(1.0/50.0);
  sc1.start_sim();
  bcp_listener listener(sc1);
  listener.start();
  std::vector<tcp_socket_p> bcps;
  for(int i=0;i<10;i++)
  {
    bcps.push_back(new_conn());
  };
  chrono::milliseconds t(500);
  this_thread::sleep_for(t);
  listener.stop();
  sc1.stop_sim();

  // let's check the results.
  int count = 0;
  triplet maxs(0);
  triplet mins(1e6,1e6,1e6);
  float r = 0.0;
  for(auto s : sc1.obj_status())
  {
    cout << s << endl;
    count++;
  };
  cout << count << endl;
  
  cout << "=========== bcp_server test complete ============="
    << endl;

failed = count != 10;    
    
  return failed;
};



























