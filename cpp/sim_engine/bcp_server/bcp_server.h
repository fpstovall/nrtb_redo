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

/**********************************************************
 * bcp_listener is the BCP connection factory for the 
 * alpha simulation engine.  It listens for connection
 * requeests from a BCP, creates a new bot_mk1 for it
 * to control and inserts the new bot into the simulation
 * engine, then promptly forgets about it. The socket 
 * listener is created and opened upon instanciation, and 
 * it is closed at destruction.  The processor thread 
 * is started and stopped by explicit methods.
 * Some parameters can be set in the global config file:
 *    bcp_port = the port number the we'll listen on for 
 *      connections; defaults to 64500.
 *********************************************************/
class bcp_listener
{
public:
  // Instanciates the class and limks to the sim_engine.
  bcp_listener(sim_core & e, std::string addr, int limit);
  // shuts down and destructs the class
  ~bcp_listener();
  // -- performance metrics
  int connections();
  int dropped();
  int pending();
  // returns true if the listener is active.
  bool listening();
  // starts the processing thread.
  void start();
  // stops the processing thread.
  void stop();
  
private:
  // used to spawn new bots on connect.
  sim_core & engine;
  // handle for the processor thread.
  std::thread p_thread;
  // stop accepting bots if this is exceeded.
  int pop_limit;
  // catches new connection requests.
  tcp_server_socket_factory listener;
  // this method runs as a separate thread.
  void processor();
};

} // namepace nrtb

#endif // bcp_server_header
