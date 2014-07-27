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
 
#include "common.h"
#include <iostream>
#include <unistd.h>

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

  returnme += report_test(
	  "nrtb::trim()",
	  nrtb::trim("\t"+tstr+"   ") == tstr);

  returnme += report_test(
	  "nrtb::dateflip()",
	  nrtb::dateflip("2011-07-21") == "07-21-2011");

  returnme += report_test(
	  "nrtb::http_hextochar()",
	  nrtb::http_hextochar("20") == " ");
  
  returnme += report_test(
	  "nrtb::http_chartohex()",
	  nrtb::http_chartohex(tstr) == "4D6958654463417345");
  
  returnme += report_test(
	  "nrtb::http_enhex()",
	  nrtb::http_enhex(tstr) == "%4D%69%58%65%44%63%41%73%45");
  
  returnme += report_test(
	  "nrtb::http_unhex()",
	  nrtb::http_unhex("%4D%69%58%65%44%63%41%73%45") == tstr);
  
  try
  {
    nrtb::base_exception e;
    e.store("Test code");
    usleep(100);
    throw e;
  }
  catch (nrtb::base_exception & e)
  {
    returnme += report_test(
      "nrtb::base_exception::comment()",
      e.comment() == "Test code");
    returnme += report_test(
      "nrtb::base_exception::age_in_ms()",
      e.age_in_ms().count() > 100 );
  };
  
  cout << "=== nrtb::common_rl unit test complete ===" << endl;
  return returnme;
};