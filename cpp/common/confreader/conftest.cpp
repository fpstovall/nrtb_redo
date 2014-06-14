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
 
/* confreader test program */

#include "confreader.h"
#include <iostream>

using namespace nrtb;
using namespace std;

log_queue lq;
log_file_writer writer(lq, "conf_test.log");

int main(int argc, char* argv[])
{
  cout.precision(10);
  bool set_if_failed = false;
  log_recorder log("config_test",lq);  
  log.info("=-=-=-=-=-= conftest Init =-=-=-=-=-=-=");
  conf_reader &  config = global_conf_reader::get_reference();

  try
  {
    log.info("Starting read");
    config.read(argc,argv,"test.config");
  }
  catch (...)
  {
    set_if_failed = true;
    cerr << "Failed reading the configuration." << endl;
    log.severe("Failed reading the configuration.");
  };
  if (config.size() != 12)
  {
    set_if_failed = true;
    cerr << "Did not find 12 parameters." << endl;
    log.severe("Did not find 12 parameters.");
  };
  // iterator test
  try
  {
    conf_reader::iterator c = config.begin();
    conf_reader::iterator e = config.end();
    while (c != e)
    {
      cout << "\t\"" <<  c->first << "\"=\"" << c->second 
	  << "\"" <<  endl;
      c++;
    };
  }
  catch (...)
  {
    set_if_failed = true;
    cerr << "Iterator test failed." << endl;
    log.severe("Iterator test failed.");
  };
  // template test.
  int test = config.get<int>("test",-1);
  int test2 = config.get<int>("test2",-1);
  string test3 = config.get<string>("test3","not specified");
  double test4 = config.get<double>("test",-1);
  double test5 = config.get<double>("test2",-1);
  cout << "(int) test = " << test 
    << "\n(int) test2 = " << test2 
    << "\n(string) test3 = \"" << test3 << "\""
    << "\n(double) test = " << test4
    << "\n(double) test2 = " << test5 
    << endl;
  if ( 
    (test != 1) 
    or (test2 != 71)
    or (test3 != "jack-danials")
    or (test4 != 1.0)
    or (test5 != 71.837486)
  )
  {
    set_if_failed = true;
    cerr << "** Template test failed." << endl;
    log.severe("** Template test failed.");
  };
  // exists test.
  cout << "?var \"--doit\" exists? " 
    << (config.exists("--doit") ? "Yes" : "No")
    << endl;
  if (!config.exists("--doit"))
  {
    set_if_failed = true;
    cerr << "exists() test failed." << endl;
    log.severe("exists() test failed.");
  };
  strlist strings = config.getall<string>("test");
  cout << "valid string \"test\" values:" << endl;
  for (unsigned int i=0; i < strings.size(); i++)
  {
    cout << "\t" << i << ": " << strings[i] << endl;
  };
  if (strings.size() != 3)
  {
    set_if_failed = true;
    cerr << "getall<string>() did not find 3 parameters." << endl;
    log.severe("getall<string>() did not find 3 parameters.");
  };
  if (set_if_failed) 
  {
    cerr << "** ntrb::conf_reader UNIT TEST FAILED. **" << endl;
    log.critical("UNIT TEST FAILED");
  };
  log.info("Run Complete");
  return set_if_failed;
};
