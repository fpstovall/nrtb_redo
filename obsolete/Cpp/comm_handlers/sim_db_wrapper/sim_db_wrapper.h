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
 
#ifndef sim_db_wrapper_header
#define sim_db_wrapper_header

#include <ack_nak.h>
#include <sim_setup_info.h>
#include <tq_update.h>
#include <sim_stop.h>
#include <sim_termination.h>
#include <sim_to_db_wrapper.pb.h>
#include <memory>
#include <stdint.h>

namespace nrtb_com {

typedef std::unique_ptr<com_ack> ack_p;
typedef std::unique_ptr<com_nak> nak_p;
typedef std::unique_ptr<com_tq_update> tq_update_p;
typedef std::unique_ptr<com_sim_setup> sim_setup_p;
typedef std::unique_ptr<com_sim_termination> sim_end_p;
typedef std::unique_ptr<com_request_sim_stop> request_stop_p;
    
class com_sim_to_db
{
public:
  
  uint32_t msg_uid;
  ack_p ack;
  nak_p nak;
  tq_update_p quanta_results;
  sim_setup_p sim_setup;
  sim_end_p sim_end;
  request_stop_p db_abort;
  
  com_sim_to_db();
  com_sim_to_db(nrtb_msg::sim_to_db * ext);
  virtual ~com_sim_to_db() {};

  void set_from_message(nrtb_msg::sim_to_db * ext);
  void load_message(nrtb_msg::sim_to_db * ext);  
  
  bool operator ==(com_sim_to_db & a);
};

} // namespace nrtb::com

#endif // ack_nak_header
