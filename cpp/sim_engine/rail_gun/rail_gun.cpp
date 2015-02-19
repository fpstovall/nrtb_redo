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
  max_roc.y = conf.get<float>("rg_traverse",pi);
  max_roc.z = conf.get<float>("rg_elevation",pi);
  max_roc.x = conf.get<float>("rg_pwr_rate",400.0);
  max_power = conf.get<float>("rg_max_pwr",2000);
  magazine = conf.get<int>("rg_rounds",100);
  // TODO: insert into parent's tick list.
  parent.register_ticker(*this);
};

rail_gun_mk1::~rail_gun_mk1()
{
  // TODO: remove from parent's tick list.
  parent.deregister_ticker(*this);
};

void rail_gun_mk1::fire(bool stable)
{

};

void rail_gun_mk1::train(triplet settings)
{

};

void rail_gun_mk1::auto_control()
{

};

rg_round::rg_round(triplet loc, triplet vel)
{

};

rg_round::~rg_round()
{

};

base_object * rg_round::clone()
{
  
};

bool rg_round::apply_collision(object_p o)
{

};

bool rg_round::check_collision(object_p o)
{
    return nrtb::base_object::check_collision(o);
};

