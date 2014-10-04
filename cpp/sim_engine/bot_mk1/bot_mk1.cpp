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

#include "bot_mk1.h"
#include <sstream>

using namespace nrtb;

bot_mk1::bot_mk1(tcp_socket_p link, triplet where)
{
  // set the physical parameters of the bot here.
  mass = 3e4;
  bounding_sphere.radius = 4.5/2.0;
  location = where;
  location.z = bounding_sphere.radius + 0.25;
  // add effectors
  add_pre(new norm_gravity);
  add_pre(new hover(location.z,0.10,2.0));
  drive.reset(new diff_steer(*this,1e5,1e6,4*pi,10,8));
  // bot control and com setup.
  BCP = std::move(link);
  ImAlive = true;
  // Start the receiver and transmitter
  r_thread = std::thread(&bot_mk1::receiver,this);
  t_thread = std::thread(&bot_mk1::transmitter,this);
  // Send ready to the BCP
  to_BCP.push("READY\r");
};
