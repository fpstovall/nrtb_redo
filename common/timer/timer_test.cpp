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
/* hirez_timer test program */

#include <iostream>
#include <unistd.h>
#include "hires_timer.h"

using namespace nrtb;
using namespace std;

int main()
{
	hirez_timer overall;
	hirez_timer interval;
	int returnme = 0;
	try
	{
	  overall.start();
	  interval.start();
	  cout << "sleep 0.1 second" << endl;
	  usleep(1e5);
	  cout << overall.interval() << " | " << interval.stop() << " (stop)" << endl;
	  cout << "sleep 3 seconds" << endl;
	  usleep(3e5);
	  cout << overall.interval() << " | " << interval.stop() << " (start)" << endl;	
	  interval.start();
	  cout << "sleep 2 seconds" << endl;
	  usleep(2e5);
	  cout << overall.interval() << " | " << interval.stop() << " (reset)" << endl;
	  interval.reset();
	  interval.start();
	  cout << "sleep 500000 useconds" << endl;
	  usleep(5e5);
	  cout << overall.interval() << " | " << interval.stop() << endl;
	  // test the advanced formationg function
	  interval.start(109472.34);
	  interval.stop();
	  cout << "Extended interval_as_HMS() test: \""
		  << interval.interval_as_HMS(true)
		  << "\" or \"" << interval.interval_as_HMS() << "\"" << endl;
	  cout << "Total run time: " << overall.stop() << " seconds." << endl;
	  if ((overall.interval() < 1.1) or (overall.interval() > 1.102))
	  {
		cerr << "Measured runtime " 
		  << overall.interval()
		  << " is outside of expected limits, failed test" << endl;
		returnme = 1;
	  };
	}
	catch (...)
	{
	  returnme = 1;
	};
	return returnme;
};

