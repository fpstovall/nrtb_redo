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
 
#include "common.h"
#include <iostream>

using namespace std;

int report_test(const string banner, bool test)
{
  string result;
  int returnme = 0;
  if (test)
  {
	result = "PASSED";
  }
  else
  {
	result = "FAILED";
	returnme = 1;
  };
  cout << "\t" << banner << ": " << result << endl;
  return returnme;
};

int main()
{
  cout << "=== Starting nrtb::common_rl unit test ===" << endl;
  int returnme = 0;
  string tstr = "MiXeDcAsE";
  
  returnme += report_test(
	  "nrtb::upcase()",
	  nrtb::upcase(tstr) == "MIXEDCASE");
  
  returnme += report_test(
	  "nrtb::downcase()",
	  nrtb::downcase(tstr) == "mixedcase");
  
  cout << "=== nrtb::common_rl unit test complete ===" << endl;
  return returnme;
};