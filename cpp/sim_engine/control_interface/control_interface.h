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
 
#ifndef control_interface_header
#define control_interface_header

#include <ipc_channel.h>
#include <base_socket.h>

namespace nrtb
{
  
enum class 
  
class ci_main_msg: public abs_ipc_record
{
public:
  enum {start,stop,status,load,shutdown};
  int msg;
  std::string arg;
};

typedef ci_main_msg * ci_main_msg_p;

class main_ci_msg: public abs_ipc_record
{
public:
  enum {start,stop,status,load,shutdown};
  
  strlist lines;
};

} // namepace nrtb

#endif // control_interface_header
