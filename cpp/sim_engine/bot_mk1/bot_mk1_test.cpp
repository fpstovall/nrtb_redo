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

#include "bot_mk1.h"
#include <iostream>
#include <string>
#include <future>

using namespace nrtb;
using namespace std;

shared_ptr<bot_mk1> test_bot;

int request_processor(tcp_server_socket_factory & server)
{
  int hits {0};
  bool done {false};
  while (!done)
  {
    try
    {
      auto sock = server.get_sock();
      // create test_bot
      test_bot.reset(new bot_mk1(std::move(sock),triplet(0.0)));
      cout << "New bot_mk1 created." << endl;
      // Update our hit count.
      hits++;
    }
    catch (tcp_server_socket_factory::queue_not_ready)
    {
      done = true;
    }
    catch (exception & e)
    {
      cout << "ReqProc recieved " 
        << typeid(e).name()
        << endl;
      done = true;
    };
  }; 
  cout << "Request processor shut down." << endl;
  return hits;
};


int main()
{
  bool failed = false;
  cout << "========== bot_mk1 test ============="
    << endl;

  // -- set up and start the listener
  string address = "localhost:64500";
  tcp_server_socket_factory test_server(address);
  test_server.start_listen();
  // start request processor
  auto rp_out = 
    async(launch::async,request_processor,std::ref(test_server));

  // -- connect (should create a bot_mk1).
  tcp_socket BCP;
  try 
  {
    BCP.connect(address);

    // first check.. did the bot say "READY"?
    string msg = gsub(BCP.getln("\r",64,2),"\r","");
    bool bad = (msg != "READY") or !(test_bot->ImAlive);
    failed = failed or bad;
    cout << "READY: " << ( bad ? "FAILED" : "Passed" ) 
      << " '" << msg << "'" << endl;
      
    // get position and velocity data.
    BCP.put("bot lvar\r");
    msg = gsub(BCP.getln("\r",64,2),"\r","");
    bad = (msg != "(0,0,2.5)(0,0,0)(0,0,0)(0,0,0)") 
      or !(test_bot->ImAlive);
    failed = failed or bad;
    cout << "bot lvar: " << ( bad ? "FAILED" : "Passed" ) 
      << " '" << msg << "'" << endl;
    
    // get health data.
    BCP.put("bot health\r");
    msg = gsub(BCP.getln("\r",64,2),"\r","");
    bad = (msg != "100") or !(test_bot->ImAlive);
    failed = failed or bad;
    cout << "bot health: " << ( bad ? "FAILED" : "Passed" ) 
      << " '" << msg << "'" << endl;
    
    // get bot error response.
    BCP.put("bot error\r");
    msg = gsub(BCP.getln("\r",64,2),"\r","");
    bad = (msg != "bad_cmd \"bot error\"") or !(test_bot->ImAlive);
    failed = failed or bad;
    cout << "bot cmd error: " << ( bad ? "FAILED" : "Passed" ) 
      << " '" << msg << "'" << endl;
    
    // get drive settings.
    BCP.put("drive status\r");
    msg = gsub(BCP.getln("\r",64,2),"\r","");
    bad = (msg != "0,1,0") or !(test_bot->ImAlive);
    failed = failed or bad;
    cout << "drive status: " << ( bad ? "FAILED" : "Passed" ) 
      << " '" << msg << "'" << endl;
    
    // change some settings.
    BCP.put("drive power 100\r");
    BCP.put("drive brake 0\r");
    BCP.put("drive turn 100\r");
    BCP.put("drive status\r");
    msg = gsub(BCP.getln("\r",64,2),"\r","");
    bad = (msg != "1,0,1") or !(test_bot->ImAlive);
    failed = failed or bad;
    cout << "drive settings: " << ( bad ? "FAILED" : "Passed" ) 
      << " '" << msg << "'" << endl;
      
    // check drive lockown on error.
    BCP.put("drive nocmd\r");
    msg = gsub(BCP.getln("\r",64,2),"\r","");
    bad = (msg != "bad_cmd \"drive nocmd\"") or !(test_bot->ImAlive);
    failed = failed or bad;
    cout << "drive cmd error: " << ( bad ? "FAILED" : "Passed" ) 
      << " '" << msg << "'" << endl;
    BCP.put("drive status\r");
    msg = gsub(BCP.getln("\r",64,2),"\r","");
    bad = (msg != "0,1,0") or !(test_bot->ImAlive);
    failed = failed or bad;
    cout << "drive lockdown: " << ( bad ? "FAILED" : "Passed" ) 
      << " '" << msg << "'" << endl;
      
    // verify running statuses.
    bad = test_bot->tick(0) or test_bot->apply(0,1.0/50.0);
    failed = failed or bad;
    cout << "tick and apply: " << ( bad ? "FAILED" : "Passed" ) 
      << endl;
    
    // clone test.
    object_p cobj(test_bot->clone());
    bad = cobj->as_str() !="ID=0:loc=(0,0,2.49804):att=(0,0,0):vel=(0,0,-0.196133):rot=(0,0,0):f=(0,0,0):t=(0,0,0):acc_mod=(0,0,0):r_mod=(0,0,0):mass=30000:mass_mod=0:b_sphere=(0,0,0),2.25:pre=gravity=(0,0,-9.80665);hover=2.5,0.1,2;diff_steer::pre=100000,0,1e+06,1,12.5664,0;:posts=diff_steer::post=10,8;";
    cout << "clone(): " << ( bad ? "FAILED" : "Passed" )
      << endl;
    failed = failed or bad;
  
    // verify bot shutdown on connection close
    BCP.close();
    chrono::milliseconds pause(20);
    this_thread::sleep_for(pause);
    bad = test_bot->ImAlive;
    failed = failed or bad;
    cout << "shudown on close: " << ( bad ? "FAILED" : "Passed" )
      << endl;
      
    // verify stopped statuses.
    bad = !test_bot->tick(0) or !test_bot->apply(0,1.0/50.0);
    failed = failed or bad;
    cout << "tick and apply: " << ( bad ? "FAILED" : "Passed" ) 
      << endl;    
      
  }
  catch (base_exception & e)
  {
    cout << "EXCEPTION: " << e.what() << "\n\t"
      << e.comment() << endl;
  }
  catch (exception & e)
  {
    cout << "EXCEPTION: " << e.what() << endl;
  }
  catch (...)
  {
    cout << "UNKNOWN EXCEPTION" << endl;
  };
  //  sender.put(sendme);    

  try { BCP.close(); } catch (...) {};
  try { test_server.stop_listen(); } catch (...) {};
    
  cout << "=========== bot_mk1 test complete ============="
    << endl;
  
  return failed;
};



























