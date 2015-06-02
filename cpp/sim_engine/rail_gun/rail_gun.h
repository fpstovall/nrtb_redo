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

#include <base_object.h>
#include <bot_interfaces.h>
#include <sim_core.h>
#include <confreader.h>
#include <hires_timer.h>

namespace nrtb
{
  
class rail_gun_mk1 : public tickable
{
public:
  rail_gun_mk1(abs_bot & p);
  virtual ~rail_gun_mk1();
  // various control methods
  virtual void fire(bool stable=true);
  // triplets are encoded as power, azimuth, delination
  virtual void train(triplet settings);
  // returns current status, goals, rounds remaining
  virtual void get_status(triplet & c, triplet & g, int & r);
  // the "tick" method
  void operator ()(float duration); 
protected:
  // parent base_object
  abs_bot & parent;
  // Simulation Engine
  sim_core & sim;
  // triplets are encoded as power, azimuth, delination
  // current state
  triplet current;
  // rate of change limits
  triplet max_roc;
  float max_power;
  // user established goals.
  triplet goals;
  // auto fire on goal achievement
  bool fire_on_ready {0};
  // number of rounds remaining.
  int magazine {100};
  // operation mutex.
  std::mutex tick_lock;
};

struct rg_round : public base_object
{
  rg_round(triplet loc, triplet vel);
  virtual ~rg_round();
  base_object * clone();
  bool apply_collision(object_p o, float duration); 
};
  
} // namepace nrtb

#endif // rail_gun_header
