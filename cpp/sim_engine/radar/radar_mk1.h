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
 
#ifndef radar_mk1_header
#define radar_mk1_header

#include <sim_core.h>
#include <bot_interfaces.h>
#include <hires_timer.h>

namespace nrtb
{
  
  
/*****************************************
 * radar_mk1 provides a simple scanner 
 * for robots.  It accesses the panopticon
 * inteface on the global_sim_core, flters
 * and formats results for the supplied 
 * object's point of view.
 ****************************************/
class radar_mk1 : public commandable
{
public:
  /***************************************
   * rader_mk1 constructor. Establishes 
   * the link to object the radar is 
   * "mounted" and the link to the 
   * global_sim_core. 
   * 
   *  o - the object the radar is mounted
   *      on. It will be used to estabish
   *      relative angles and ranges.
   **************************************/
  radar_mk1(object_p o);
  // NOP distructor for safety.
  virtual ~radar_mk1() {};
  // command interface.
  bool command(std::string cmd, std::string & response);
private:
  object_p parent;
  sim_core & sim;
  /***************************************
   * Returns a string containing information
   * on each contact.  
   * The string starts with an integer 
   * indicating the number of contects 
   * listed, followed by a group for each
   * contact where:
   *   type - int indicating target type.
   *   polar vector - range,elevation,azmuth
   *                  to target
   *   rel velocity - cartisian 3D relative
   *                  velocity to target
   **************************************/
  std::string get_contacts();
  // In this version always returns "1"
  std::string status();
};

} // namepace nrtb

#endif // radar_mk1_header
