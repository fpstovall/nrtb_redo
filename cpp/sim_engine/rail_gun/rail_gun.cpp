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

// see rail_gun.h for documentation

#include "rail_gun.h"
#include <sstream>
#include <iostream>

using namespace nrtb;

rail_gun_mk1::rail_gun_mk1(abs_bot& p)
  : parent(p), sim(global_sim_core::get_reference())
{
  conf_reader & conf = global_conf_reader::get_reference();
  max_roc.y = conf.get<float>("rg_traverse",pi*8);
  max_roc.z = conf.get<float>("rg_elevation",pi*8);
  max_roc.x = conf.get<float>("rg_pwr_rate",400.0);
  max_power = conf.get<float>("rg_max_pwr",4000);
  magazine = conf.get<int>("rg_rounds",100);
  // insert into parent's tick list.
  parent.register_ticker(*this);
};

rail_gun_mk1::~rail_gun_mk1()
{
  // remove from parent's tick list.
  parent.deregister_ticker(*this);
};

void rail_gun_mk1::fire(bool stable)
{
  // if stable set fire_on_ready;
  if (stable) { fire_on_ready = true; }
  else
  {
    // if not, unconditionally fire
    // == create new rg_round
    // == add to global_sim_core
    // == zero current.x (power vector);
    // == remove one round from the magazine;
  }
};

void rail_gun_mk1::train(triplet settings)
{
  goals = settings;
};

void rail_gun_mk1::operator()(float duration)
{
  /*******************************
   * Assumes the following:
   *    goals are within limits.
   *    
   ******************************/
  triplet d_roc = max_roc * duration;
  // step power towards goal.
  if (current.x != goals.x)
  {
    float sign = (current.x < goals.x) ? 1 : -1;
    float delta  = fabs(goals.x - current.x);
    current.x = (delta > d_roc.x) ? (current.x + (d_roc.x*sign)) : goals.x;
  };
  // step azumith towards goal.
  if (current.y != goals.y)
  {
    float sign = (current.y < goals.y) ? 1 : -1;
    float delta  = fabs(goals.y - current.y);
    current.y = (delta > d_roc.y) ? (current.y + (d_roc.y*sign)) : goals.y;    
  };
  // step elevation towards goal.
  if (current.z != goals.z)
  {
    float sign = (current.z < goals.z) ? 1 : -1;
    float delta  = fabs(goals.z - current.z);
    current.z = (delta > d_roc.z) ? (current.z + (d_roc.z*sign)) : goals.z;    
  };
  // at goal and fire on ready?
  if (fire_on_ready and (current == goals)) fire(false);
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

bool rg_round::apply_collision(object_p o, float duration)
{

};


