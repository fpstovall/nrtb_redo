/* hirez_timer test program */

#include <iostream>
#include <unistd.h>
#include <nrtb_timer.h>

using namespace NRTB;
using namespace std;

int main()
{
	hirez_timer overall;
	hirez_timer interval;
	overall.start();
	interval.start();
	cout << "sleep 1 second" << endl;
	sleep(1);
	cout << overall.interval() << " | " << interval.stop() << " (stop)" << endl;
	cout << "sleep 3 seconds" << endl;
	sleep(3);
	cout << overall.interval() << " | " << interval.stop() << " (start)" << endl;	
	interval.start();
	cout << "sleep 2 seconds" << endl;
	sleep(2);
	cout << overall.interval() << " | " << interval.stop() << " (reset)" << endl;
	interval.reset();
	interval.start();
	cout << "sleep 500000 useconds" << endl;
	usleep(500000);
	cout << overall.interval() << " | " << interval.stop() << endl;
	// test the advanced formationg function
	interval.start(109472.34);
	interval.stop();
	cout << "Extended interval_as_HMS() test: \""
		<< interval.interval_as_HMS(true)
		<< "\" or \"" << interval.interval_as_HMS() << "\"" << endl;
	cout << "Total run time: " << overall.interval() << " seconds." << endl;
	return 0;
};

