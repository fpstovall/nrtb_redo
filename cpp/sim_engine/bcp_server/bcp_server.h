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
 
#ifndef bcp_server_header
#define bcp_server_header

#include <base_socket.h>
#include <confreader.h>
#include <common_log.h>
#include <bot_mk1.h>
#include <sim_core.h>

namespace nrtb
{

class bcp_listener
{
public:
  bcp_listener(sim_core & e);
  ~bcp_listener();
  
  int connections();
  bool listening();
  void stop();
  
private:
  sim_core & engine;
  std::thread p_thread;
  tcp_server_socket_factory listener;
  void processor();
};

} // namepace nrtb

#endif // bcp_server_header
