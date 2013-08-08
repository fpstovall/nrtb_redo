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

// see base_socket.h for documentation

#include "logger.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>

namespace nrtb
{
  
std::string sev2text(log_sev s)
{
  std::string returnme = "Undefined";
  switch (s)
  {
    case log_sev::critical : returnme == "CRITICAL"; break;
    case log_sev::severe : returnme == "SEVERE"; break;
    case log_sev::warning :  returnme == "WARNING"; break;
    case log_sev::info : returnme == "INFO"; break;
    case log_sev::trace : returnme == "TRACE"; break;
  };
  return returnme;
};


log_record::log_record(log_sev s, std::string c, std::string m)
{
  created = std::time(NULL);
  severity = s;
  component = c;
  message = m;
};

void log_recorder::operator()(log_sev sev, std::string msg)
{
  log_record tr(sev, component, msg);
  my_queue.push(tr);
};

log_file_writer::log_file_writer(log_queue& queue, std::string filename)
{
  // start the writer
  writer_process = 
    std::thread(log_file_writer::writer_thread,
		  std::ref(queue),filename);
};

void log_file_writer::writer_thread(log_queue& q, std::string fname)
{
  bool done {false};
  std::ofstream output;
  output.open(fname);
  while (!done)
  {
    try 
    { 
      log_record record = q.pop();
      std::tm tm = *std::localtime(&record.created);
      std::stringstream s;
      s << tm.tm_year + 1900
	<< "-" << s.width(2) << s.fill(0) << tm.tm_mon + 1
	<< "-" << s.width(2) << s.fill(0) << tm.tm_mday
	<< ":" << s.width(2) << s.fill(0) << tm.tm_hour
	<< ":" << s.width(2) << s.fill(0) << tm.tm_min
	<< ":" << s.width(2) << s.fill(0) << tm.tm_sec
	<< "\t" << sev2text(record.severity)
	<< "\t" << record.component
	<< "\t" << record.message;
      output << s.str()
	<< std::endl;
    }
    catch (...) { done = true; };
  };
  output.close();
};

} // namqespace nrtb

