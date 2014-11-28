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

// see base_socket.h for documentation

#include "rail_gun.h"
#include <sstream>
#include <iostream>

using namespace nrtb;

rail_gun_mk1::rail_gun_mk1(abs_bot& p)
  : parent(p), sim(global_sim_core::get_reference())
{
  conf_reader & conf = global_conf_reader::get_reference();
  max_traverse_rate = conf.get<float>("rg_traverse",pi*8);
  max_elevation_rate = conf.get<float>("rg_elevation",pi*8);
  max_power_rate = conf.get<float>("rg_pwr_rate",100.0);
  magazine = conf.get<int>("rg_rounds",100);
  control = std::thread(&rail_gun_mk1::auto_control,this);
};

rail_gun_mk1::~rail_gun_mk1()
{
  // order control to shut down;
  // TODO: How do I do this?
  // wait for control to shut down;
  if (control.joinable()) control.join();
};

