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
 
#ifndef sim_messages_header
#define sim_messages_header

#include <ipc_channel.h>
#include <memory>


namespace nrtb
{

typedef std::shared_ptr<void> void_p;

class gp_sim_message : public abs_ipc_record
{
public:
  gp_sim_message(ipc_queue & q, int t, int n, int v);
  gp_sim_message(ipc_queue & q, int t, int n, int v, void_p d);
  int msg_type();
  std::string as_str();
  int noun();
  int verb();
  template <class T>
    T & data();
private:
  int _noun;
  int _verb;
  int _type;
  void_p _data;
};

typedef gp_sim_message * gp_sim_message_p;


template <class T>
  T & gp_sim_message::data()
{
  return *(static_cast<T*>(_data.get()));
};


} // namepace nrtb

#endif // sim_messages_header
