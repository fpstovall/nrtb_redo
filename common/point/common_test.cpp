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

int main()
{
	ld_triad a(1,2,3);
	ld_triad b(3,2,1);

	cout << setprecision(10);
	cout << "=== Triad Test ===" << endl;
	cout << "\ta = " << a << "; b = " << b << "\n" << endl;

	cout << "\ta + b = " << a + b << endl;
	cout << "\ta - b = " << a - b << endl;
	cout << "\ta * b = " << a * b << endl;
	cout << "\ta / b = " << a / b << endl;
	cout << "\ta += b; a = " << (a += b) << endl;
	cout << "\ta -= b; a = " << (a -= b) << endl;
	cout << "\ta *= b; a = " << (a *= b) << endl;
	cout << "\ta /= b; a = " << (a /= b) << endl;
	cout << "\n\ta.pow(b) = " << a.pow(b) << endl;
	cout << "\ta.range(b) = " << a.range(b) << endl;
	cout << "\ta.magnatude() = " << a.magnatude() << endl;
	cout << "\n\ta == a = " << (a == a) << endl;
	cout << "\ta == b = " << (a== b) << endl;
	cout << "\ta != b = " << (a != b) << endl;
	cout << "\ta != a = " << (a != a) << endl;

	cout << endl;

	cout << "\ta + 2 = " << a + 2 << endl;
	cout << "\ta - 2 = " << a - 2 << endl;
	cout << "\ta * 2 = " << a * 2 << endl;
	cout << "\ta / 2 = " << a / 2 << endl;
	cout << "\ta += 2; a = " << (a += 2) << endl;
	cout << "\ta -= 2; a = " << (a -= 2) << endl;
	cout << "\ta *= 2; a = " << (a *= 2) << endl;
	cout << "\ta /= 2; a = " << (a /= 2) << endl;
	cout << "\n\ta.pow(2) = " << a.pow(2) << endl;
	cout << "\ta.pow(0.5) = " << a.pow(0.5) << endl;
	cout << "\ta.normalize() = " << a.normalize() << endl;
	cout << "\ta.normalize().magnatude() = "<< a.normalize().magnatude() << endl;
	
	cout << "\n\a\tInput a new value for b (x,y,z): " << flush;
	cin >> b;
	cout << "\t\tb = " << b << endl;
	cout << "\tb.to_str() =\"" << b.to_str() << "\"" << endl;
	cout << "\tb.from_str(b.to_str(10)) = " << b.from_str(b.to_str(10)) << endl;


	cout << "\n=== Test Complete ===" << endl;
	return 0;
	
};
