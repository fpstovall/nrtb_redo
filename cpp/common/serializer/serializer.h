/***********************************************
 This file is part of the NRTB project (https://github.com/fpstovall/nrtb_beta).
 
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

#include <atomic>

namespace nrtb 
{ 
  /******************************************************************
   * nrtb::serializer provides a simple, thread safe functor which
   * returns a series of long long ints increasing by one each time
   * it's called. By default it starts counting from zero, but may
   * be started from any arbitrary integer in the range 
   * 0 < x < max long long. 
   * ***************************************************************/
  class serializer
  {
  public:
	// default constructor; counter starts from zero.
	serializer();
	// constructor which sets the starting number.
	serializer(unsigned long long start);
	// NOP distructor for inheritance safety
	virtual ~serializer();
	// functor method, returns the next value in the sequence.
	unsigned long long operator ()();
  private:
	std::atomic<unsigned long long> counter;
  };

}

#endif // nrtb_serializer_h