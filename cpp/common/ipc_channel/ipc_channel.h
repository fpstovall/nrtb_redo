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
 
#ifndef ipc_channel_header
#define ipc_channel_header

#include <common.h>
#include <linear_queue.h>
#include <memory>
#include <singleton.h>


namespace nrtb
{
  
class abs_ipc_record;

typedef std::unique_ptr<abs_ipc_record> ipc_record_p;

typedef linear_queue<ipc_record_p> ipc_queue;

/** abs_ipc_record is the base for all ipc messages.
 * This abstract includes a return_address (the
 * ipc_queue the a return should be put to). but 
 * no other payload. It'll need to be overriden to 
 * provide whatever payload a given channel may 
 * need.
 */
class abs_ipc_record
{
public:
  ipc_queue & return_to;
  abs_ipc_record(ipc_queue & q);
};

/** ipc_channel_manager provides a place to stash ipc_queues
 * so that their scope is not tied to the lifetime of any
 * particular thread.  Ideally, this should be used as s 
 * singleton class.
 */
class ipc_channel_manager
{
public:
  typedef std::map<std::string,ipc_queue> channel_List;
  typedef channel_List::iterator iterator;
  
  ipc_queue & get(std::string name);
  iterator begin();
  iterator end();
  
private:
  channel_List channels;
};

/// And here is the singleton class to make ipc channels safe.
typedef singleton<ipc_channel_manager> global_ipc_channel_manager;

} // namepace nrtb

#endif // ipc_channel_header
