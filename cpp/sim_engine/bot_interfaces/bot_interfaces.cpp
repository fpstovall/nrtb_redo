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


#include "bot_interfaces.h"
#include <sstream>
#include <iostream>

using namespace nrtb;

void ticker::register_ticker(tickable& t)
{
  tickees.emplace((unsigned long long) &t, t);
};

void ticker::deregister_ticker(tickable& t)
{
  tickees.erase((unsigned long long) &t);
};

void ticker::tick_all(float duration)
{
  for(auto p : tickees) 
    p.second(duration);
};

bool abs_bot::tick(int quanta, float duration)
{
  tick_all(duration);
  return nrtb::base_object::tick(quanta, duration);
}
