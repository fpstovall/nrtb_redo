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

#include "bcp_server.h"
#include <iostream>
#include <string>
#include <future>

using namespace nrtb;
using namespace std;

shared_ptr<bot_mk1> test_bot;


int main()
{
  bool failed = false;
  cout << "========== bcp_server test ============="
    << endl;

  // -- set up and start the listener
  string address = "localhost:64500";
  tcp_server_socket_factory test_server(address);
//  test_server.start_listen();
  // start request processor
//  auto rp_out = 
//    async(launch::async,request_processor,std::ref(test_server));

  // -- connect (should create a bot_mk1).
  tcp_socket BCP;
    
  cout << "=========== bcp_server test complete ============="
    << endl;

failed = true;    
    
  return failed;
};



























