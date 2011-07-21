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
  
  returnme += report_test(
	  "nrtb::gsub()",
	  nrtb::gsub(tstr, "cAsE", " frogs") == "MiXeD frogs");

  // split() testing
  nrtb::strlist tokens = nrtb::split("this is a test",' ');
  bool faults = tokens.size() == 4;
  faults = faults or (tokens[0] == "this");
  faults = faults or (tokens[0] == "is");
  faults = faults or (tokens[0] == "a");
  faults = faults or (tokens[0] == "test");  
  returnme += report_test(
	"nrtb::split()",
	faults);
  
  string tchar = " "; tchar[0] = 0;
  returnme += report_test(
	  "nrtb::mconvert()",
	  nrtb::mconvert("\\ \" '"+tchar) == "\\\\ \\\" \\\'\\0");


  cout << "=== nrtb::common_rl unit test complete ===" << endl;
  return returnme;
};