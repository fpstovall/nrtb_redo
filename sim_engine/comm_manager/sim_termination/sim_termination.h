/***********************************************
 This file is part of the NRTB project (https://launchpad.net/nrtb).
 
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
 
#ifndef sim_setup_header
#define sim_setup_header

#include <sim_management.pb.h>
#include <boost/shared_ptr.hpp>

namespace nrtb_com {

class com_sim_termination
{
public:
  uint32_t last_quanta;
  std::string reason;
  uint32_t error_code;
  float avg_cook_ms;
  float avg_quanta_ms;
  
  com_sim_termination();
  com_sim_termination(nrtb_msg::sim_termination_data * ext);
  virtual ~com_sim_termination() {};

  void set_from_message(nrtb_msg::sim_termination_data * ext);
  void load_message(nrtb_msg::sim_termination_data * ext);  
  
  bool operator ==(com_sim_termination & a);
};

} // namespace nrtb::com

#endif // sim_setup_header
