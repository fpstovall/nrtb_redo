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

// see base_socket.h for documentation

#include "railgun.h"
#include <confreader.h>
#include <sstream>
#include <iostream>

using namespace nrtb;

railgun::railgun()
  : sim(global_sim_core::get_reference())
{
  conf_reader & conf = global_conf_reader::get_reference();
};

bool railgun::command(std::string cmd, std::string& response)
{
  bool returnme = false;
  response = "";
  std::stringstream s(cmd);
  std::string module;
  std::string verb;
  s >> module >> verb;
  if (module == "railgun")
  {
    returnme = true;
    if (verb == "contacts") { response = get_contacts(); }
    else if (verb == "status") { response = status(); }
    else 
    {
      // not a recognised command.
      response = "bad cmd \""+cmd+"\"";
    };
  };
  return returnme;
};

bool railgun_mk1::tick(base_object & o, float quanta)
{
};

