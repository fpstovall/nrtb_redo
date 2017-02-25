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

#include "radar_mk1.h"
#include <confreader.h>
#include <sstream>
#include <iostream>

using namespace nrtb;

radar_mk1::radar_mk1()
  : sim(global_sim_core::get_reference())
{
  conf_reader & conf = global_conf_reader::get_reference();
  dist_return_limit = conf.get<float>("radar_dr",10000.0);
  type_return_limit = conf.get<float>("radar_tr",10000.0);
};

bool radar_mk1::command(std::string cmd, std::string& response)
{
  bool returnme = false;
  response = "";
  std::stringstream s(cmd);
  std::string module;
  std::string verb;
  s >> module >> verb;
  if (module == "radar")
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

std::string radar_mk1::get_contacts()
{
  std::stringstream returnme;
  std::stringstream results;
  int counter = 0;
  // for now, update every time.
  contacts c_list = sim.contact_list();
  // assemble the return string
  returnme << "radar contacts ";
  if (c_list.size())
  {
    // returnme << (c_list.size()-1);
    for(auto c : c_list)
    {
      if (c.id != parent_id)
      {
        // float range = parent_location.range(c.location);
        // get xy azimuth
        triplet offset = (c.location - parent_location).to_polar();
        // see if we need to do range and type adjustments
        if (offset.x > dist_return_limit)
        {
          // create range inaccuracy.
          float adjustment = offset.x - dist_return_limit;
          float scale = (adjustment - floor(adjustment)*1.0) - 0.5;
          offset.x += (adjustment * scale);
          // hide velocity
          c.velocity = parent_velocity;
        };
        if (offset.x > type_return_limit) { c.type = 0; };
        // send it out if we are close enough.
        if (offset.x < (dist_return_limit*5.0))
        {
          counter++;
          // assemble return string
          results << " " << c.type << " "
            << offset << " "
            << (c.velocity - parent_velocity);
        };
      };
    };
    returnme << counter << results.str();
  }
  else returnme << 0;
  return returnme.str();
};

std::string radar_mk1::status()
{
  return "radar status 1";
};

bool radar_mk1::tick(base_object & o, float quanta)
{
	parent_id = o.id;
	parent_location = o.location;
	parent_velocity = o.velocity;
	return false;
};

