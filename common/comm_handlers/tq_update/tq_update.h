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
 
#ifndef tq_update_header
#define tq_update_header

#include <vector>
#include <memory>
#include <location_data.h>
#include <velocity_data.h>
#include <tq_obj.h>
#include <obj_setup_info.h>
#include <sim_obj_tq_update.pb.h>

namespace nrtb_com {

class com_tq_update
{
public:
  typedef std::unique_ptr<nrtb_com::com_tq_obj_update> up_p;
  typedef std::unique_ptr<nrtb_com::com_obj_setup> obj_p;
  typedef std::vector<nrtb_com::com_tq_obj_update> up_list;
  typedef std::vector<nrtb_com::com_obj_setup> ob_list;
  uint32_t quanta_id;
  uint32_t sim_uid;
  up_list obj_data;
  ob_list new_objs;
  
  com_tq_update();
  com_tq_update(nrtb_msg::tq_data * ext);
  virtual ~com_tq_update() {};

  void set_from_message(nrtb_msg::tq_data* ext);
  void load_message(nrtb_msg::tq_data* ext);  
  
  bool operator ==(com_tq_update & a);
};

} // namespace nrtb::com

#endif // triplet_header
