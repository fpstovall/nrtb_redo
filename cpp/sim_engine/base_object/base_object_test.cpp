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

#include "base_object.h"
#include <iostream>
#include <string>

using namespace nrtb;
using namespace std;

typedef shared_ptr<string> string_p;

int main()
{
  cout << "=========== sim messages test ============="
    << endl;

  ipc_queue queue;
    
  void_p s(new string("this is a test"));
  
  queue.push(ipc_record_p(new gp_sim_message(queue, 1, 1, 0)));
  queue.push(ipc_record_p(new gp_sim_message(queue, 2, 1, 1, s)));
  
  ipc_record_p raw(queue.pop());
  gp_sim_message_p msg = static_cast<gp_sim_message_p>(raw.get());
  cout << msg->as_str() << endl;
  bool failed = (msg->as_str() != "1:1:0:0");
  
  raw = queue.pop();
  msg = static_cast<gp_sim_message_p>(raw.get());
  cout << msg->as_str() << endl;
  failed = failed
    or (msg->msg_type() != 2)
    or (msg->noun() != 1)
    or (msg->verb() != 1)
    or (msg->data<string>() != "this is a test");

  cout << "=========== sim_messages test complete ============="
    << endl;
  
  return failed;
};



























