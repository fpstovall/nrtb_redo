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

#include <iostream>
#include <sstream>
#include "base_socket.h"
#include "../../../dev/common/threads/base_thread.h"

using namespace nrtb;
using namespace std;

class myserver: public tcp_server_socket_factory
{
	private:
		int hits;
		mutex data;
	protected:
		// on_accept() is called on each connection.
		void on_accept()
		{
			try
			{
				// we'll just return the current time in seconds 
				string temp = ricklib::itos(time(NULL));
				connect_sock->put(temp+"\n");
				// Close the socket.
				connect_sock->close();
				// delete the tcp_socket (it was new'd before we were called)
				delete connect_sock;
					// Update our hit count. 
				scope_lock lock(data);
				hits++;
			}
			catch (...)
			{
				std::cerr << "Unexpected error in on_accept()" << std::endl;
			};
		};
	public:
		// constructor
		myserver(const string & a, const unsigned short int & b) 
			: tcp_server_socket_factory(a,b)
		{
			// Don't need to lock here because we know the 
			// listener thread is not running.
			hits = 0;
		};
		// hit count reporter.
		int hit_count() 
		{
			scope_lock lock(data); 
			return hits; 
		};
};






























