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
#include <hires_timer.h>

namespace nrtb
{
  
class radar_mk1
{
public:
  radar_mk1(base_object & o);
  virtual ~radar_mk1() {};
  std::string contacts();
  std::string status();
private:
  base_object & parent;
  sim_core & sim;
  contacts_p c_list;
};

} // namepace nrtb

#endif // radar_mk1_header
