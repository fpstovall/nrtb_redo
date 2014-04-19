/***********************************************
 This file is part of the NRTB project (https://*launchpad.net/nrtb).
 
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

#include "sim_core.h"
#include <sstream>

using namespace nrtb;

void sim_core::tick(unsigned long long quanta)
{
  // call the local tick and apply for each object in the simulation.
  for(auto & a: all_objects)
  {
    // have to use this method to make sure the apply step is not
    // optimized out by the compiler in some conditions.
    bool killme = a.second->tick(quanta);
    bool killme2 = a.second->apply(quanta, quanta_duration);
    // mark the object for deletion if appropriate.
    if (killme or killme2)
      deletions.push_back(a.first);
  };
  // at the end of this method, all objects are either
  // at their final state ignorning collisions or deleted.
};

void sim_core::collision_check()
{
  // this version is using a very naive algorithm.
  auto b = all_objects.begin();
  auto c = b;
  auto e = all_objects.end();
  while (c != e)
  {
    b = c;
    b++;
    while (b != e)
    {
      if (c->second->check_collision(b->second))
      {
        // a collision has been found.
        clsn_rec crec;
        crec.a = c->first;
        crec.b = b->first;
        collisions.push_back(crec);
      };
      b++;
    };
    c++;
  };
  // at exit, all colliions have been recorded.
};


void sim_core::turn_init(unsigned long long quanta)
{
  /* conduct all cleanup needed to ensure the sim
     is a proper state for the next turn. */
  // Clear out the collision list
  collisions.clear();
  // delete any objects marked in the last turn.
  for (auto a : deletions)
  {
    all_objects.erase(a);
  };
  // clear the deletions list
  deletions.clear();
  // update the quanta counter
  quanta++;
  /*********************
   * Still need to add message handling.
   * *******************/
   //TODO: much more here, but need the messaging
   // first.
};

void sim_core::put_message(gp_sim_message_p m)
{
  ipc_record_p t(static_cast<abs_ipc_record *>(m.release()));
  messages.push(std::move(t));
};

gp_sim_message_p sim_core::next_out_message()
{
  ipc_record_p t(messages.pop());
  gp_sim_message_p g(static_cast<gp_sim_message *>(t.release()));
  return std::move(g);
};