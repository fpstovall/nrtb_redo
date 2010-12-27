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

#include "Poco/SimpleFileChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/Logger.h"
#include "Poco/AutoPtr.h"

using Poco::SimpleFileChannel;
using Poco::FormattingChannel;
using Poco::PatternFormatter;
using Poco::Logger;
using Poco::AutoPtr;

void nrtb::setup_global_logging(const std::string & logfilename)
{
  AutoPtr<SimpleFileChannel> pFile(new SimpleFileChannel);
  pFile->setProperty("path", logfilename);
  pFile->setProperty("rotation", "2 K");
  AutoPtr<PatternFormatter> pPF(new PatternFormatter);
  pPF->setProperty("pattern", "%Y-%m-%d %H:%M:%S %s: %t");
  AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pFile));
  Logger::root().setChannel(pFC);
  Logger::root().notice("Logging system initialized");
}