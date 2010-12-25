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
 
/* confreader test program */

#include "confreader.h"
#include <iostream>

using namespace nrtb;
using namespace std;

int main(int argc, char* argv[])
{
	cout << "Confreader Init" << endl;
	conf_reader & config = conf_reader::get_instance();
	cout << "Starting read" << endl;
	cout << "Read " << flush << config.read(argc,argv,"test.config")
		<< " unique arguments from CL and file test.config" << endl;
	conf_reader::iterator c = config.begin();
	conf_reader::iterator e = config.end();
	while (c != e)
	{
		cout << "\t\"" <<  c->first << "\"=\"" << c->second 
			<< "\"" <<  endl;
		c++;
	};
	// template test.
	int test = config.get<int>("test",-1);
	int test2 = config.get<int>("test2",-1);
	string test3 = config.get<string>("test3","not specified");
	cout << "(int) test = " << test 
		 << "\n(int) test2 = " << test2 
		 << "\n(string) test3 = " << test3 << endl;
	double test4 = config.get<double>("test",-1);
	double test5 = config.get<double>("test2",-1);
	cout << "(double) test = " << test4
		 << "\n(double) test2 = " << test5 
		 << endl;
	cout << "?var \"--doit\" exists? " 
		<< (config.exists("--doit") ? "Yes" : "No")
		<< endl;	
	vector<int> intlist = config.getall<int>("test");
	cout << "valid int \"test\" values:" << endl;
	for (unsigned int i=0; i < intlist.size(); i++)
	{
		cout << "\t" << i << ": " << intlist[i] << endl;
	};
	strlist strings = config.getall<string>("test");
	cout << "valid string \"test\" values:" << endl;
	for (unsigned int i=0; i < strings.size(); i++)
	{
		cout << "\t" << i << ": " << strings[i] << endl;
	};
	cout << "\nRun Complete" << endl;
};
