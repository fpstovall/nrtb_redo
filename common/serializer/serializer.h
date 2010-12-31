/***********************************************
 T his file is part of the NRTB project (https://*launchpad.net/nrtb).
 
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

#ifndef nrtb_serializer_h
#define nrtb_serializer_h
#include <boost/concept_check.hpp>

#include <Poco/Mutex.h>

namespace nrtb 
{ 

  class serializer
  {
  public:
	serializer();
	serializer(unsigned long long start);
	~serializer();
	unsigned long long operator ()();
  private:
	Poco::Mutex lock;
	unsigned long long int counter;
  };

}

#endif // nrtb_serializer_h