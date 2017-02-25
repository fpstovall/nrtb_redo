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

#include "sim_db_wrapper.h"

#include <iostream>

namespace nrtb_com
{
  
com_sim_to_db::com_sim_to_db()
{
  // No actions needed.
};

com_sim_to_db::com_sim_to_db(nrtb_msg::sim_to_db* ext)
{
  set_from_message(ext);
};

void com_sim_to_db::set_from_message(nrtb_msg::sim_to_db* ext)
{
  msg_uid = ext->msg_uid();
  // check each payload type to see if it's included
  if (ext->has_ack())
	ack.reset(new com_ack(ext->mutable_ack()));
  else 
	ack.reset();
  // nak
  if (ext->has_nak())
	nak.reset(new com_nak(ext->mutable_nak()));
  else 
	nak.reset();
  // quanta update
  if (ext->has_quanta_results())
	quanta_results.reset(new com_tq_update(ext->mutable_quanta_results()));
  else
	quanta_results.reset();
  // sim setup
  if (ext->has_sim_setup())
	sim_setup.reset(new com_sim_setup(ext->mutable_sim_setup()));
  else
	sim_setup.reset();
  // simulation normal end
  if (ext->has_sim_end())
	sim_end.reset(new com_sim_termination(ext->mutable_sim_end()));
  else
	sim_end.reset();
  // simulation end request from the data broker
  if (ext->has_db_abort())
	db_abort.reset(new com_request_sim_stop(ext->mutable_db_abort()));
  else
	db_abort.reset();
};

void com_sim_to_db::load_message(nrtb_msg::sim_to_db* ext)
{
  ext->set_msg_uid(msg_uid);
  // check each payload and process as needed.
  if (ack)
	ack->load_message(ext->mutable_ack());
  else
	ext->clear_ack();
  // nak
  if (nak)
	nak->load_message(ext->mutable_nak());
  else
	ext->clear_nak();
  // time quanta update
  if (quanta_results)
	quanta_results->load_message(ext->mutable_quanta_results());
  else
	ext->clear_quanta_results();
  // simulation setup
  if (sim_setup)
	sim_setup->load_message(ext->mutable_sim_setup());
  else
	ext->clear_sim_setup();
  // simulation normal end
  if (sim_end)
	sim_end->load_message(ext->mutable_sim_end());
  else
	ext->clear_sim_end();
  // simulation end request from data broker
  if (db_abort)
	db_abort->load_message(ext->mutable_db_abort());
  else
	ext->clear_db_abort();  
};

bool com_sim_to_db::operator==(com_sim_to_db& a)
{
  bool returnme = msg_uid == a.msg_uid;
  // ack
  if ((ack) and (a.ack))
	returnme = (*ack == *a.ack) && returnme;
  else if (!((!ack) and (!a.ack)))
	returnme = false;
  // nak
  if ((nak) and (a.nak))
	returnme = (*nak == *a.nak) && returnme;
  else if (!((!nak) and (!a.nak)))
	returnme = false;
  // time quanta update
  if ((quanta_results) and (a.quanta_results))
	returnme = (*quanta_results == *a.quanta_results) && returnme;
  else if (!((!quanta_results) and (!a.quanta_results)))
	returnme = false;
  // simulation setup
  if ((sim_setup) and (a.sim_setup))
	returnme = (*sim_setup == *a.sim_setup) && returnme;
  else if (!((!sim_setup) and (!a.sim_setup)))
	returnme = false;
  // simulation normal stop
  if ((sim_end) and (a.sim_end))
	returnme = (*sim_end == *a.sim_end) && returnme;
  else if (!((!sim_end) and (!a.sim_end)))
	returnme = false;
  // simulation end request from data broker
  if ((db_abort) and (a.db_abort))
	returnme = (*db_abort == *a.db_abort) && returnme;
  else if (!((!db_abort) and (!a.db_abort)))
	returnme = false;
};

} // namespace nrtb::com

