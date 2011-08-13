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

// singleton template test program

#include "singleton.h"
#include <serializer.h>
#include <iostream>

using namespace nrtb;
using namespace std;

typedef singleton<serializer> sequence_type;

int main()
{
  
  cout << "============== singleton unit test ================" 
	<< endl;
  int er_count = 0;
  
  sequence_type & a = sequence_type::get_instance();
  
  for (int i=0; i<10; i++)
  {
	cout << a();
  };
  
  sequence_type & b = sequence_type::get_instance();
  
  if ( b() != 10)
  {
	er_count++;
  };
  
  cout << "\n=========== singleton test " << (er_count ? "failed" : "passed")
	<< " =============" << endl;

  return er_count;
};
