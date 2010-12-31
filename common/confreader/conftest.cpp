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
#include "../include/log_setup.h"
#include <iostream>
#include "Poco/Logger.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/AutoPtr.h"

using namespace nrtb;
using namespace std;

int main(int argc, char* argv[])
{
  bool set_if_failed = false;
  setup_global_logging("conf_test.log");
  Poco::Logger & log = Poco::Logger::get("conftest");
  log.information("=-=-=-=-=-= conftest Init =-=-=-=-=-=-=");
  conf_reader & config = conf_reader::get_instance();
  try
  {
	log.information("Starting read");
	config.read(argc,argv,"test.config");
  }
  catch (...)
  {
	set_if_failed = true;
	cerr << "Failed reading the configuration." << endl;
  };
  if (config.size() != 12)
  {
	set_if_failed = true;
	cerr << "Did not find 12 parameters." << endl;
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
	(test != 1) or (test2 != 0)
	or (test3 != "jack danials")
	or (test4 != 1.0) or (test5 != 71.837486)
  )
  {
	set_if_failed = true;
	cerr << "** Template test failed." << endl;
  };
  // exists test.
  cout << "?var \"--doit\" exists? " 
	  << (config.exists("--doit") ? "Yes" : "No")
	  << endl;
  if (!config.exists("--doit"))
  {
	set_if_failed = true;
	cerr << "exists() test failed." << endl;
  };
  vector<int> intlist = config.getall<int>("test");
  cout << "valid int \"test\" values:" << endl;
  for (unsigned int i=0; i < intlist.size(); i++)
  {
	  cout << "\t" << i << ": " << intlist[i] << endl;
  };
  if (intlist.size() != 2)
  {
	set_if_failed = true;
	cerr << "getall<int>() did not find 2 parameters." << endl;
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
  };
  if (set_if_failed) 
  {
	cerr << "** ntrb::conf_reader UNIT TEST FAILED. **" << endl;
	log.fatal("UNIT TEST FAILED");
  };
  log.information("Run Complete");
  return set_if_failed;
};
