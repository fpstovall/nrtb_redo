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
 
#ifndef sim_stop_header
#define sim_stop_header

#include <sim_management.pb.h>

namespace nrtb_com {

class com_request_sim_stop
{
public:
  bool failed;
  std::string reason;
  
  com_request_sim_stop();
  com_request_sim_stop(nrtb_msg::db_request_sim_stop * ext);
  virtual ~com_request_sim_stop() {};

  void set_from_message(nrtb_msg::db_request_sim_stop * ext);
  void load_message(nrtb_msg::db_request_sim_stop * ext);  
  
  bool operator ==(com_request_sim_stop & a);
};

} // namespace nrtb::com

#endif // sim_stop_header
