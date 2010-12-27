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
 
#include "log_setup.h"

#include <string>
#include <Poco/Logger.h>
#include "Poco/LogStream.h"

using namespace std;

int main()
{
  nrtb::setup_global_logging("test_ouput.log");
  Poco::Logger & logger = Poco::Logger::get("log_test");
  logger.notice("Logging should be set up now.");
  Poco::LogStream log(logger);
  log << "This message used the stream interface" << endl;
  logger.fatal("Program run complete.");
}
