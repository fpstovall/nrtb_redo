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

// see base_object.h for documentation

#include "bcp_server.h"
#include <random>
#include <sstream>

using namespace nrtb;

bcp_listener::bcp_listener(sim_core& e)
  : engine(e),
    listener("*:"+
      global_conf_reader::get_reference().get<std::string>(
        "bcp_port","64500")),
    pop_limit(global_conf_reader::get_reference().get<int>(
        "pop_limit",100))
{};

bcp_listener::~bcp_listener()
{
  try { stop(); } catch (...) {};
};

void bcp_listener::start()
{
  listener.start_listen();
  p_thread = std::thread(&bcp_listener::processor,this);
};

void bcp_listener::stop()
{
  try { listener.stop_listen(); } catch (...) {};
  p_thread.join();
};

bool bcp_listener::listening()
{
  return listener.listening();
};

int bcp_listener::connections()
{
  return listener.accepted();
};

int bcp_listener::dropped()
{
  return listener.discarded();
};

int bcp_listener::pending()
{
  return listener.available();
};

void bcp_listener::processor()
{
  int rejected = 0;
  auto log = common_log::get_reference()("BCP_req_proc");
  log.trace("Starting");
  try
  {
    std::mt19937 rng;
    std::uniform_int_distribution<int> u(0,2e4);
    while (listener.listening())
    {
      tcp_socket_p bcp = listener.get_sock();
      if (engine.size() < pop_limit)
      {
        log.trace("new connection");
        triplet l(u(rng)-1e4,u(rng)-1e4,0.0);
        engine.add_object(object_p(new bot_mk1(std::move(bcp), l)));     
        log.trace("bot added to sim");
      }
      else 
      {
        // fully populated, reject.
        bcp->put("NOT_AVAILALE\r");
        rejected++;
      };
    };
  }
  catch (base_exception& e)
  {
    if (e.comment() != "")
    {
      std::stringstream s;
      s << e.what() << ":" << e.comment();
      log.warning(s.str());
    }
    else
    {
      log.trace("shutdown requested");
    };
  }
  catch (std::exception& e)
  {
    log.severe(e.what());
  }
  catch (...)
  {
    log.critical("Unidentified exception");
  };
  std::stringstream s;
  s << "connections: " << connections()
    << ", rejected: " << rejected
    << ", dropped: " << dropped();
  log.info(s.str());
  log.trace("Exiting");
};



