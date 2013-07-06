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
 
 /*** Test program for triad.h *****/

#include <unistd.h>
#include <iostream>
#include <iomanip>
#include "triad.h"

using namespace std;

typedef nrtb::triad<long double> ld_triad;

int test_triad(const std::string prompt, ld_triad val, ld_triad right, int ec)
{
  cout << "\t" << prompt << " = " << val << endl;
  if (val != right)
  {
    ec++;
    cerr << "\t\tTest Failed: Answer should be " << val << endl;
  }
  return ec;
};

int test_ld(const std::string prompt, long double val, long double right, int ec)
{
  cout << "\t" << prompt << " = " << val << endl;
  if (val != right)
  {
    ec++;
    cerr << "\t\tTest Failed: Answer should be " << val << endl;
  }
  return ec;
};

int main()
{
  ld_triad a(1,2,3);
  ld_triad b(3,2,1);
  int returnme = 0;

  cout << setprecision(10);
  cout << "=== nrtb::triad Unit Test ===" << endl;
  cout << "\ta = " << a << "; b = " << b << endl;
  // basic operations tests
  returnme = test_triad("a + b",a + b,ld_triad(4,4,4),returnme);
  returnme = test_triad("a - b",a - b,ld_triad(-2,0,2),returnme);
  returnme = test_triad("a * b",a * b,ld_triad(3,4,3),returnme);
  returnme = test_triad("a / b",a / b,ld_triad(1.0d/(long double) 3.0,1,3),returnme);
  returnme = test_triad("a += b; a",a += b,ld_triad(4,4,4),returnme);
  returnme = test_triad("a -= b; a",a -= b,ld_triad(1,2,3),returnme);
  returnme = test_triad("a *= b; a",a *= b,ld_triad(3,4,3),returnme);
  returnme = test_triad("a /= b; a",a /= b,ld_triad(1,2,3),returnme);
  // power test
  returnme = test_triad("a.pow(b)",a.pow(b),ld_triad(1,4,3),returnme);
  // range test
  ld_triad t = b - a;
  t *= t;
  long double r = sqrt(t.x + t.y + t.z);
  returnme = test_ld("a.range(b)",a.range(b),r,returnme);
  // magnatude test
  returnme = test_ld("a.magnatude()",a.magnatude(),a.range(0),returnme);
  // boolean tests
  returnme = test_ld("a == a",a == a,1,returnme);
  returnme = test_ld("a == b",a == b,0,returnme);
  returnme = test_ld("a != b",a != b,1,returnme);
  returnme = test_ld("a != a",a != a,0,returnme);
  // point/scalar operations
  returnme = test_triad("a + 2",a + 2,ld_triad(3,4,5),returnme);
  returnme = test_triad("a - 2",a - 2,ld_triad(-1,0,1),returnme);
  returnme = test_triad("a * 2",a * 2,ld_triad(2,4,6),returnme);
  returnme = test_triad("a / 2",a / 2,ld_triad(0.5,1,1.5),returnme);
  returnme = test_triad("a += 2",a += 2,ld_triad(3,4,5),returnme);
  returnme = test_triad("a -= 2",a -= 2,ld_triad(1,2,3),returnme);
  returnme = test_triad("a *= 2",a *= 2,ld_triad(2,4,6),returnme);
  returnme = test_triad("a /= 2",a /= 2,ld_triad(1,2,3),returnme);
  returnme = test_triad("a.pow(2)",a.pow(2),ld_triad(1,4,9),returnme);
  // normalization test
  cout << "\ta.normalize() = " << a.normalize() << endl;
  returnme = test_ld("a.normalize().magnatude()",a.normalize().magnatude(),1.0,returnme);
  // dot and vector product tests.
  returnme = test_ld("a.dot_product(b)",a.dot_product(b),10,returnme);
  returnme = test_triad("a.vector_product(b)",a.vector_product(b),
	  ld_triad(-4,8,-4),returnme);
  // string i/o tests, assumes "2,3.5,7) is input.
  cout << "\tInput a new value for b \"(2,3.5,7)\": " << flush;
  cin >> b; cout << endl;
  returnme = test_triad("b",b,ld_triad(2,3.5,7),returnme);
  returnme = test_triad("b.from_str(b.to_str(10))",
				  b.from_str(b.to_str(10)),ld_triad(2,3.5,7),returnme);
  // report errors, if any
  if (returnme)
  {
    cerr << "There were " << returnme 
      << " error(s) found." << endl;
  }
  cout << "=== nrtb::triad Unit Test Complete ===" << endl;
  // return the error count as the exit code
  return returnme;
	
};
