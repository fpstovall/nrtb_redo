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

#include "sim_termination.h"

#include <iostream>

namespace nrtb_com
{

com_sim_termination::com_sim_termination()
{
  last_quanta = 0;
  reason = "";
  error_code = 0;
  avg_cook_ms = 0.0;
  avg_quanta_ms = 0.0;
};

com_sim_termination::com_sim_termination(nrtb_msg::sim_termination_data* ext)
{
  set_from_message(ext);
};

void com_sim_termination::set_from_message(nrtb_msg::sim_termination_data* ext)
{
  last_quanta = ext->last_quanta();
  reason = ext->reason();
  error_code = ext->error_code();
  if (ext->has_avg_cook_ms())
	avg_cook_ms = ext->avg_cook_ms();
  else
	avg_cook_ms = 0;
  if (ext->has_avg_quanta_ms())
	avg_quanta_ms = ext->avg_quanta_ms();
  else
	avg_quanta_ms = 0;
};

void com_sim_termination::load_message(nrtb_msg::sim_termination_data* ext)
{
  ext->set_last_quanta(last_quanta);
  ext->set_reason(reason);
  ext->set_error_code(error_code);
  if (avg_cook_ms > 0)
	ext->set_avg_cook_ms(avg_cook_ms);
  if (avg_quanta_ms)
	ext->set_avg_quanta_ms(avg_quanta_ms);
};

bool com_sim_termination::operator==(com_sim_termination& a)
{
  bool returnme = true;
  returnme = (last_quanta == a.last_quanta) and returnme;
  returnme = (reason == a.reason) and returnme;
  returnme = (error_code == a.error_code) and returnme;
  returnme = (avg_cook_ms == a.avg_cook_ms) and returnme;
  returnme = (avg_quanta_ms == a.avg_quanta_ms) and returnme;
  return returnme;
};

  
} // namespace nrtb::com

