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

#include "sim_stop.h"

#include <iostream>

namespace nrtb_com
{

com_request_sim_stop::com_request_sim_stop()
{
  failed = false;
  reason = "";
};

com_request_sim_stop::com_request_sim_stop(nrtb_msg::db_request_sim_stop* ext)
{
  set_from_message(ext);
};

void com_request_sim_stop::set_from_message(nrtb_msg::db_request_sim_stop* ext)
{
  failed = ext->failed();
  if (ext->has_reason())
	reason = ext->reason();
  else
	reason = "";
};

void com_request_sim_stop::load_message(nrtb_msg::db_request_sim_stop* ext)
{
  ext->set_failed(failed);
  if (reason != "")
	ext->set_reason(reason);
};

bool com_request_sim_stop::operator==(com_request_sim_stop& a)
{
  bool returnme = true;
  returnme = (failed == a.failed) and returnme;
  returnme = (reason == a.reason) and returnme;
  return returnme;
};

  
} // namespace nrtb::com

