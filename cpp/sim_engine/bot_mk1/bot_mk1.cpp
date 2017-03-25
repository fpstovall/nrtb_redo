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

// see base_object.h for documentation

#include "bot_mk1.h"
#include <sstream>

using namespace nrtb;

bot_mk1::bot_mk1(triplet where)
{
  std::stringstream s;
  s << "mk1_" << id;
  name = s.str();
  // set the physical parameters of the bot here.
  mass = 3e4;
  bounding_sphere.radius = 4.5/2.0;
  location = where;
  location.z = bounding_sphere.radius + 0.25;
};

bot_mk1::bot_mk1(tcp_socket_p link, triplet where)
  : bot_mk1(where)
{
  // add effectors
	add_pre(std::make_shared<radar_mk1>());
  add_pre(std::make_shared<norm_gravity>());
  add_pre(std::make_shared<hover>(location.z,0.10,2.0));
  diff_steer(*this,1e5,2e5,4*pi,10,8);
  // bot control and com setup.
  BCP = std::move(link);
  ImAlive = true;
  // Start the receiver and transmitter
  r_thread = std::thread(&bot_mk1::receiver,this);
  t_thread = std::thread(&bot_mk1::transmitter,this);
  cp_thread = std::thread(&bot_mk1::msg_router,this);
  // Send ready to the BCP
  to_BCP.push("READY");
};

object_p bot_mk1::clone()
{
  bot_mk1 * t = new bot_mk1(location);
  t->pre_attribs = get_pre_attribs_copy();
  t->post_attribs = get_post_attribs_copy();
  t->id = id;
  t->location = location;
  t->velocity = velocity;
  t->attitude.set(attitude);
  t->rotation.set(rotation);
  t->ImAlive.store(ImAlive);
  return object_p(t);
};

bot_mk1::~bot_mk1()
{
  ImAlive = false;
  // unconditionally shutdown the queue and close the socket.
  try { to_BCP.shutdown(); } catch (...) {};
  try { from_BCP.shutdown(); } catch (...) {};
  if (BCP) try { BCP->close(); } catch (...) {};
  // Wait on the threads.
  if (r_thread.joinable()) r_thread.join();
  if (t_thread.joinable()) t_thread.join();
  if (cp_thread.joinable()) cp_thread.join();
};

bool bot_mk1::tick(float duration)
{
  if (ImAlive)
  {
    return nrtb::abs_bot::tick(duration);
  }
  else
  {
    // we are dead.. tell sim_engine.
    return true;
  };
};

bool bot_mk1::apply(float quanta)
{
  if (ImAlive)
  {
    bool returnme = nrtb::base_object::apply(quanta);
    return returnme;
  }
  else
  {
    // we are dead.. tell sim_engine.
    return true;
  };
};

bool bot_mk1::check_collision(object_p o, float duration)
{
  if (ImAlive)
  {
    return nrtb::base_object::check_collision(o, duration);
  }
  else
  {
    // we are dead.. tell sim_engine.
    return true;
  };
};

bool bot_mk1::apply_collision(object_p o, float duration)
{
  return false;
};

void bot_mk1::receiver()
{
  auto & conf = global_conf_reader::get_reference();
  int timeout = conf.get<int>("bcp_timeout",60);
  auto log(common_log::get_reference()(name));
  try
  {
    while (ImAlive)
    {
      std::string s;
      s = BCP->getln("\r",64,timeout);
      // strip CR and LF.
      s = gsub(gsub(s,"\n",""),"\r","");
      if (s.length() > 0)
      {  
        log.trace("<< "+s);
        from_BCP.push(s);
      };
    };
  }
  catch (...)
  {
    ImAlive = false;
  };
};

void bot_mk1::transmitter()
{
  auto log(common_log::get_reference()(name));
  try
  {
    while (ImAlive)
    {
      std::string s = to_BCP.pop();
      BCP->put(s+"\r");
      log.trace(">> "+s);
    };
  }
  catch (...)
  {
    ImAlive = false;
  };
};

void bot_mk1::msg_router()
{
  std::string s = "";
  while (true) 
  {
    try {s = from_BCP.pop(); } catch (...) { return; };
    try
    {
      // tick notification.
      if (s == "tick")
      {
        gonculate();
      }
      else
      {
        std::string returnme = "";
        // check local commands first.
        std::stringstream tokens(s);
        std::string sys;
        std::string verb;
        tokens >> sys >> verb;
        if (sys == "bot")
        {
          if (verb == "lvar")
          {
            lvar();
          }
          else if (verb == "health")
          {
            health();
          }
          else if (verb == "ping")
          {
            to_BCP.push("READY");
          }
          else if (verb == "autopilot")
          {
            autopilot(tokens);
          }
          else
          {
            to_BCP.push("bad_cmd \""+s+"\"");
          };
        }
        // check for effector commands.
        else if (command(s,returnme))
        {
          if (returnme != "") to_BCP.push(returnme);
        }
        else
        {
          to_BCP.push("bad_sys \""+s+"\"");
        };
      };
    }
    catch (...)
    {
      to_BCP.push("WTF? \""+s+"\"");
    };
  }; // while true
};

void bot_mk1::send_to_bcp(std::string msg)
{
  to_BCP.push(msg);
};

void bot_mk1::bot_cmd(std::string cmd)
{
  auto log(common_log::get_reference()(name));
  log.trace("++ "+cmd);
  from_BCP.push(cmd);
};

void bot_mk1::lock()
{
  from_BCP.hold();
};

void bot_mk1::unlock()
{
  from_BCP.release();
  from_BCP.push("tick");
};

void bot_mk1::gonculate()
{
  if (auto_on)
  {
    // check speed and adjust
    triplet v = velocity;
    v.z = 0;
    float delta = v.magnatude() - set_speed;
    std::stringstream s;
    s << "drive power ";
    if (delta > 0.1) { s << -power_limit; } 
    else if (delta >= 0) { s << 0; }
    else { s << power_limit; };
    from_BCP.push(s.str());
    // check heading and adjust
    std::stringstream t;
    t << "drive turn ";
    float current_heading = attitude.angles().z;
    delta = (current_heading - set_heading);
    if ((delta < 0.0) or (delta > 3.14159)) { t << turn_limit; }
    else { t << -turn_limit; };
    from_BCP.push(t.str());
  };
};

void bot_mk1::lvar()
{
  std::stringstream s;
  s << "bot" << " " << "lvar" 
  << " " << location 
  << " " << velocity
  << " " << attitude.angles() 
  << " " << rotation.angles();
  to_BCP.push(s.str());
};

void bot_mk1::health()
{
  to_BCP.push("bot health 100");
};

void bot_mk1::autopilot(std::stringstream & s)
{
  try
  {
    std::string verb;
    s >> verb;
    if (verb == "on") 
    { 
      auto_on = true;
    }
    else if (verb == "off")
    {
      auto_on = false;
    }
    else if (verb == "power")
    {
      s >> power_limit;
    }
    else if (verb == "speed")
    {
      s >> set_speed;
    }
    else if (verb == "turn_rate")
    {
      s >> turn_limit;
    }
    else if (verb == "heading")
    {
      s >> set_heading;
      set_heading = fmod(set_heading,3.14159*2);
    }
    else if (verb == "status")
    {
      std::stringstream response;
      response << "bot autopilot "
        << auto_on << " "
        << set_heading << " "
        << set_speed << " "
        << turn_limit << " "
        << power_limit;
      to_BCP.push(response.str());
    }
    else 
    {
      to_BCP.push("bot autopilot bad_cmd");
    };
  }
  catch (...) 
  {
    to_BCP.push("bot autopilot WTF?");
  };
};
