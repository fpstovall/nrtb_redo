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

#include "logger.h"
#include <sstream>
#include <iostream>
#include <boost/concept_check.hpp>

using namespace nrtb;
using namespace std;

int ecount {0};

string stext_test(string a, log_sev b)
{
  stringstream s;
  s << a  << " " << (a == sev2text(b) ? "passed" : "failed");
  if (a != sev2text(b)) ecount++;
  return s.str();
};

int main()
{
  cout << "=========== logger unit test ============="
    << endl;

  log_queue test_logger;
  log_recorder log("Unit_Test",test_logger);
  log_file_writer writer(test_logger,"test_log.log");
  
  log(log_sev::trace, "Starting");
  for(auto i= 0; i<10; i++)
  {
    stringstream s;
    s << "Log entry " << i;
    log(log_sev::info,s.str());
  };
  
  log(log_sev::trace,"sev2test() testing");
  log(log_sev::info, stext_test("CRITICAL",log_sev::critical));
  log(log_sev::info, stext_test("SEVERE",log_sev::severe));
  log(log_sev::info, stext_test("WARNING",log_sev::warning));
  log(log_sev::info, stext_test("INFO",log_sev::info));
  log(log_sev::info, stext_test("TRACE",log_sev::trace));
  
  log(log_sev::trace,"Shutting down");
  writer.close();
  
  bool good = (test_logger.in_count == test_logger.out_count);
  good = good and (ecount == 0);
  
  // test status billboard
  cout << test_logger.in_count
    << " : " << test_logger.out_count
    << " : " << ecount
    << endl;
  if (!good)
    cout << "Errors reported.. check log file." << endl;
  cout << "=========== tcp_socket and server test complete ============="
    << endl;
  
  return !good; // return code has reversed values.
};



























