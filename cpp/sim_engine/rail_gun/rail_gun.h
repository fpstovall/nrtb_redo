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
 
#ifndef rail_gun_header
#define rail_gun_header

#include <bot_mk1.h>
#include <sim_core.h>
#include <hires_timer.h>

namespace nrtb
{
  
class rail_gun_mk1
{
public:
  rail_gun_mk1(bot_mk1 & p);
  virtual ~rail_gun_mk1();
  std::string cmd_processor(std::string cmd);
private:
  // parent base_object
  bot_mk1 & parent;
  // current state
  float azimuth;
  float elevation;
  float power;
  // rate of change limits
  float max_traverse_rate;
  float max_elevation_rate;
  float max_power_rate;
  // selected rate of change
  float set_traverse_rate;
  float set_elevation_rate;
  // user established goals.
  float azimuth_goal;
  float elevation_goal;
  float power_goal;
  // auto fire on goal achievement
  bool fire_on_ready;
  // number of rounds remaining.
  int magazine;
  // -- methods
  // moves towards goals
  
};
  
} // namepace nrtb

#endif // rail_gun_header
