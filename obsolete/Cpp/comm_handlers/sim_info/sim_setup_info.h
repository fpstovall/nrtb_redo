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
 
#ifndef sim_setup_header
#define sim_setup_header

#include <obj_setup_info.h>
#include <sim_management.pb.h>
#include <vector>
#include <memory>

namespace nrtb_com {

class com_sim_setup
{
public:
  std::string name;
  std::string model;
  uint32_t quanta_ms;
  uint32_t max_quantas;
  std::string started_by;
  typedef std::unique_ptr<com_obj_setup> obj_p;
  typedef std::vector<obj_p> obj_list;
  obj_list items;
  
  com_sim_setup();
  com_sim_setup(nrtb_msg::sim_setup_data * ext);
  virtual ~com_sim_setup() {};

  void set_from_message(nrtb_msg::sim_setup_data* ext);
  void load_message(nrtb_msg::sim_setup_data* ext);  
  
  bool operator ==(com_sim_setup & a);
};

} // namespace nrtb::com

#endif // sim_setup_header
