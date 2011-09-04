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
 
#include "base_thread.h"
#include <iostream>
#include <sstream>
#include "./tests/loop_counter.h"
#include "./tests/general.h"
#include "./tests/cond_var.h"

using namespace std;
using namespace nrtb;

void runnable_pair_test(runnable & a, runnable & b)
{
  cerr << "Entering runnable_pair_test" << endl;
  static thread ut1, ut2;
  ut1.start(a);
  ut2.start(b);
  ut1.join();
  ut2.join();
};

int main()
{
  int ec_count = 0;
  // runnable work packet test.
  counter_work_packet cwp1, cwp2;
  cwp2.count_to = 1500;
  runnable_pair_test(cwp1, cwp2);
  runnable_pair_test(cwp2, cwp1);
  cout << cwp1() << "\n" << cwp2() << endl;
  // check the results.
  if ((cwp1.loop_counter != 2000) or (cwp1.call_counter != 2))
  {
	ec_count++;
  };
  if ((cwp2.loop_counter != 3000) or (cwp2.call_counter != 2))
  {
	ec_count++;
  };

  // general thread test
  gen_tester gen_test;
  gen_test.run();
  cout << "\nGeneral thread test\n" << gen_test() << endl;
  ec_count += gen_test.ec;

  // conditional variable tests
  cond_test cv_test;
  runnable_pair_test(store, cv_test);
  cout << cv_test() << endl;
  ec_count += cv_test.ec;
  
  if (ec_count)
  {
	cerr << "*** thread_test failed with " << ec_count
	  << " errors" << endl;
  };
  
  return ec_count;
};