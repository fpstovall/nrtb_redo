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

#include "ipc_channel.h"
#include <iostream>
#include <future>

using namespace nrtb;
using namespace std;

class main_msg: public abs_ipc_record
{
public:  
  using abs_ipc_record::abs_ipc_record;
  int msg_num;
};

typedef main_msg * main_msg_p;

class worker_msg: public abs_ipc_record
{
public:
  using abs_ipc_record::abs_ipc_record;
  int ret_num;
};

typedef worker_msg * worker_msg_p;

int worker(int limit)
{
  cout << "worker started" << endl;
  global_ipc_channel_manager & ipc
    = global_ipc_channel_manager::get_reference();
  ipc_queue & in = ipc.get("worker");
  ipc_queue & out = ipc.get("main");
  int total(0);
  while (total < limit)
  {
    ipc_record_p raw_msg = in.pop();
    main_msg_p msg = static_cast<main_msg_p>(raw_msg.get());
    worker_msg_p outmsg(new worker_msg(in));
    outmsg->ret_num = msg->msg_num;
    msg->return_to.push(ipc_record_p(outmsg));
    total++;
  };
  cout << "worker ended" << endl;
  return total;
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
  auto worktask = async(launch::async,worker,limit);
  
  for (int i(0); i<limit; i++)
  {
    main_msg_p msg(new main_msg(in));
    msg->msg_num = i;
    out.push(ipc_record_p(msg));
  };
  
  cout << "processed " << worktask.get() << endl;

  while (in.size())
  {
    ipc_record_p raw = in.pop();
    worker_msg_p reply = static_cast<worker_msg_p>(raw.get());
  };
  
  auto b = ipc.begin();
  auto e = ipc.end();
  for (auto i = b; i != e; i++)
  {
    cout << i->first << ": "
      << i->second.in_count << " | "
      << i->second.out_count << endl;
  }

  cout << "=========== IPC Channel test complete ============="
    << endl;

  return !(out.in_count == in.out_count);
};



























