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

#include "diff_steer.h"
#include <sstream>
#include <iostream>

using namespace nrtb;

abs_effector * diff_steer_pre::clone()
{
  return new diff_steer_pre(*this);
};

std::string diff_steer_pre::as_str()
{
  std::stringstream s;
  // TODO: Complete the status string
  s << "diff_steer_pre=" << "TODO";
  return s.str();
};

bool diff_steer_pre::tick(base_object& o, int time)
{
  // TODO: just a filler.
  return false;
};

abs_effector * diff_steer_post::clone()
{
  return new diff_steer_post(*this);
};

std::string diff_steer_post::as_str()
{
  std::stringstream s;
  // TODO: Complete the status string
  s << "diff_steer_post=" << "TODO";
  return s.str();
};

bool diff_steer_post::tick(base_object& o, int time)
{
  // TODO: just a filler.
  return false;
};

diff_steer::diff_steer(base_object& o, float thrust, float turn_rate, float skid_friction, float slide_friction)
{
  // TODO: Create and attach the effectors to the object.
};

float diff_steer::drive(float power)
{
  // TODO: update the local variable and the pre-effector;
};

float diff_steer::brake(float braking)
{
  // TODO: update the local variable and the pre-effector;
};

float diff_steer::turn(float rate)
{
  // TODO: update the local variable and the pre-effector;
};

float diff_steer::get_drive()
{
  return drive_val;
};

float diff_steer::get_brake()
{
  return brake_val;
};

float diff_steer::get_turn()
{
  return turn_val;
};


