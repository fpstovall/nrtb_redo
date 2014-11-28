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

#include <bot_interfaces.h>
#include <sim_core.h>
#include <confreader.h>
#include <hires_timer.h>

namespace nrtb
{
  
class rail_gun_mk1
{
public:
  rail_gun_mk1(abs_bot & p);
  virtual ~rail_gun_mk1();
  virtual void cmd_processor(std::string cmd);
protected:
  // parent base_object
  abs_bot & parent;
  // Simulation Engine
  sim_core & sim;
  // current state
  float azimuth {0.0};
  float elevation {0.0};
  float power {0.0};
  // rate of change limits
  float max_traverse_rate {0.0};
  float max_elevation_rate {0.0};
  float max_power_rate {0.0};
  // user established goals.
  float azimuth_goal {0.0};
  float elevation_goal {0.0};
  float power_goal {0.0};
  // auto fire on goal achievement
  bool fire_on_ready {0};
  // number of rounds remaining.
  int magazine {100};
  // operation mutex.
  std::mutex tick_lock;
  // thread handle
  std::thread control;
  // -- methods
  // ticker; moves towards goals
  virtual void auto_control();
  // various control methods
  virtual void fire(bool stable=true);
  virtual void train(float az, float el);
  virtual void set_power(float p);
};
  
} // namepace nrtb

#endif // rail_gun_header
