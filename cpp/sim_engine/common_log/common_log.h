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
 
#ifndef sim_common_log_header
#define sim_common_log_header

#include <logger.h>
#include <singleton.h>

/***************************************
 * Provides easy access to log recorders 
 * for modules using the global sim_engine
 * log. To get a log_recorder for your 
 * module us the following syntax:
 * 
 * log_recorder mylog(common_log::get_reference()("moduleName"));
 * 
 * See the unit test in common/logger for
 * more information on using the log facility.
 **************************************/
 
namespace nrtb
{

class log_factory
{
public:
  log_recorder operator () (std::string s);
};

typedef singleton<log_factory> common_log;

} // namespace nrtb

#endif // sim_common_log_header
