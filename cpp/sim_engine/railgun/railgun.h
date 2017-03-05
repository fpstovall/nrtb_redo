/***********************************************
 This file is part of the NRTB project (https://github.com/fpstovall/nrtb_beta).
 
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
 
#ifndef railgun_header
#define railgun_header

#include <sim_core.h>
#include <bot_interfaces.h>
#include <hires_timer.h>

namespace nrtb
{
  
/*****************************************
 ****************************************/
class railgun : public abs_effector
{
public:
  /***************************************
   **************************************/
  railgun();
  // NOP distructor for safety.
  virtual ~railgun() {};
  // command interface.
  bool command(std::string cmd, std::string & response);
  bool commandable() { return true; };
  std::string as_str() { return "railgun"; };
  bool tick(base_object & o, float quanta);
  effector_p clone() { return std::make_shared<railgun_mk1>(); };
private:
  sim_core & sim;
};

} // namepace nrtb

#endif // railgun_header
