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
 
#ifndef logger_header
#define logger_header

#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <linear_queue.h>

namespace nrtb
{

enum class log_sev {critical,severe,warning,info,trace};

std::string sev2text(log_sev s);

struct log_record
{
  log_record(log_sev s, std::string c, std::string m);
  std::time_t created; 
  log_sev severity;
  std::string component;
  std::string message;
};

typedef linear_queue<log_record> log_queue;

class log_recorder
{
public:
  log_recorder(std::string comp, log_queue & queue): 
    component(comp), my_queue(queue) {};
  void operator () (log_sev sev, std::string msg);
  void critical(std::string msg);
  void severe(std::string msg);
  void warning(std::string msg);
  void info(std::string msg);
  void trace(std::string msg);
private:
  log_queue & my_queue;
  std::string component;
};

class log_file_writer
{
public:
  log_file_writer(log_queue & queue, std::string filename);
  ~log_file_writer();
  void close();
private:
  log_queue & myqueue;
  std::thread writer_process;
  static void writer_thread(log_queue & q, std::string fname);
};

} // namepace nrtb

#endif // logger_header
