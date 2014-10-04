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
 
#ifndef bot_mk1_header
#define bot_mk1_header

#include <base_object.h>
#include <base_socket.h>
#include <diff_steer.h>
#include <hover.h>
#include <gravity.h>
#include <linear_queue.h>

namespace nrtb
{

struct bot_mk1: public base_object
{
  bot_mk1(tcp_socket_p link, triplet where);
  virtual ~bot_mk1() {};
  // polymophic copier
  virtual base_object * clone() = 0;
  // additional data
  std::atomic<bool> ImAlive;
  std::shared_ptr<diff_steer> drive;
  tcp_socket_p BCP;
  linear_queue<std::string> to_BCP;
  std::mutex cooking_lock;
  std::thread r_thread;
  std::thread t_thread;
  // reporting
  virtual std::string as_str();
  // sim methods
  // returns true if a collision is detected.
  virtual bool check_collision(object_p o);
  // the following return true if the object is destroyed.
  virtual bool tick(int time);
  virtual bool apply(int time, float quanta);
  virtual bool apply_collision(object_p o);
  // BCP communications managers;
  void receiver();
  void transmitter();
};

} // namepace nrtb

#endif // bot_mk1_header
