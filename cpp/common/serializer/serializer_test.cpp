/***********************************************
 T his file is part of the NRTB project (https://github.com/fpstovall/nrtb_alpha).
 
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
 
#include "serializer.h"
#include <iostream>

using namespace nrtb;
using namespace std;

int main()
{
  cout << "** nrtb::serializer unit test **" << endl;
  bool set_if_failed = false;
  serializer set_default;
  serializer set_start(20);
  serializer rollover((unsigned long long)(0 - 1));
  cout << "Default initialization" << endl;
  cout << " " << set_default();
  cout << " " << set_default();
  cout << " " << set_default();
  cout << endl;
  if (set_default() != 3)
	set_if_failed = true;
  cout << "Started from twenty" << endl;
  cout << " " << set_start();
  cout << " " << set_start();
  cout << " " << set_start();
  cout << endl;
  if (set_start() != 23)
	set_if_failed = true;
  cout << "Rollover Demonstration" << endl;
  cout << " " << rollover();
  cout << " " << rollover();
  cout << " " << rollover();
  cout << endl;
  if (rollover() != 2)
	set_if_failed = true;
  if (set_if_failed) 
	cout << "UNIT TEST FAILED" << endl;
  return set_if_failed;
};