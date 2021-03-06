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
 
#ifndef sim_messages_header
#define sim_messages_header

#include <ipc_channel.h>
#include <memory>


namespace nrtb
{

// Used by gp_sim_message to carry extra data.
typedef std::shared_ptr<void> void_p;

/****************************************************
 * gp_sim_message is the format for all messages into
 * and out of the simulation engine. 
 *   msg_type : the message category
 *   noun: What to work on.
 *   verb: What to do with it.
 *   data: a shared_ptr<void> to an optional data packet.
 * None of the data elements may be changed after the
 * message is created.
 ****************************************************/
class gp_sim_message : public abs_ipc_record
{
public:
  // construct without a data package.
  gp_sim_message(ipc_queue & q, int t, int n, int v);
  // construct with a data packet.
  gp_sim_message(ipc_queue & q, int t, int n, int v, void_p d);
  // returns the message type  
  int msg_type();
  // returns a string approximation of the message
  // -- mostly for logging and testing.
  std::string as_str();
  // returns the noun value of the message
  int noun();
  // returns the verb value of the message
  int verb();
  // returns a copy of the data packet.
  // -- be careful to use the correct T, or the 
  // -- results are undefined and probably BAD.
  template <class T>
    T data();
private:
  int _noun;
  int _verb;
  int _type;
  void_p _data;
};

// Pointer used to manage messages in a message queue.
typedef std::unique_ptr<gp_sim_message> gp_sim_message_p;

/***************************************************
 * gp_sim_message_adapter is used to put gp_sim_messages
 * into and remove them from an ipc_queue. It's just 
 * a convienence class which handles the required type 
 * casts for you.
 **************************************************/
class gp_sim_message_adapter
{
public:
  // constructs the adaptor and attaches it to a queue
  gp_sim_message_adapter(ipc_queue & _q);
  // Push a message pointer on the queue.
  void push(gp_sim_message_p m);
  // pop a message pointer off the queue.
  gp_sim_message_p pop();
protected:
  ipc_queue & q;
};


template <class T>
  T gp_sim_message::data()
{
  if (_data.get())
  {
    return *(static_cast<T*>(_data.get()));
  }
  else
  {
    // null pointer.. very bad juju!
    base_exception e;
    e.store("Null pointer in gp_sim_message::data()");
    throw e;
  };
};


} // namepace nrtb

#endif // sim_messages_header
