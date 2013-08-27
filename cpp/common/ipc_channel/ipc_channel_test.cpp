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

#include "ipc_channel.h"
#include <iostream>

using namespace nrtb;
using namespace std;

class main_msg: public abs_ipc_record
{
public:  
  using abs_ipc_record::abs_ipc_record;
  int msg_num;
};

class worker_msg: public abs_ipc_record
{
public:
  using abs_ipc_record::abs_ipc_record;
  int ret_num;
};

int worker(int limit)
{
  global_ipc_channel_manager & ipc 
    = global_ipc_channel_manager::get_reference();
  ipc_queue & in = ipc.get("worker");
  ipc_queue & out = ipc.get("main");
  int total(0);
  while (total < limit)
  {
    main_msg & msg = static_cast<main_msg>(in.pop());
    worker_msg_p outmsg(new worker_msg(in));
    outmsg->ret_num = msg->msg_num;
    out.push(outmsg);
    total++;
  };
};

int main()
{
  cout << "=========== IPC Channel test ============="
    << endl;
    
  global_ipc_channel_manager & ipc 
    = global_ipc_channel_manager::get_reference();
  ipc_queue & in = ipc.get("main");
  ipc_queue & out = ipc.get("worker");
  int limit = 100;
  // start the worker here.
  
  for (int i(0); i<limit; i++)
  {
    main_msg_p msg(new main_msg(in));
    msg->msg_num = i;
    out.push(msg);
  };

  cout << "=========== IPC Channel test complete ============="
    << endl;
  
};



























