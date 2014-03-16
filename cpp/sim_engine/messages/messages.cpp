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

// see base_socket.h for documentation

#include "messages.h"
#include <sstream>

using namespace nrtb;
  
gp_sim_message::gp_sim_message(ipc_queue& q, int t, int n, int v)
  : abs_ipc_record(q), _noun (n), _verb (v), _type (t)
{
  _data.reset();
};

gp_sim_message::gp_sim_message(ipc_queue & q, int t,
				     int n, int v, void_p d)
: abs_ipc_record(q), _noun (n), _verb (v), _type (t), _data (d)
{
  // no actions to take.
};

int gp_sim_message::msg_type()
{
  return _type;
};

std::string gp_sim_message::as_str()
{
  std::stringstream t;
  t << _type << ":"
    << _noun << ":"
    << _verb << ":"
    << _data.get();
  return t.str();
};

int gp_sim_message::noun()
{
  return _noun;
};

int gp_sim_message::verb()
{
  return _verb;
};