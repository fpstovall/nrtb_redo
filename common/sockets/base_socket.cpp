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

// see base_socket.h for documentation

#include "base_socket.h"
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <boost/lexical_cast.hpp>

using boost::lexical_cast;
using std::string;

namespace nrtb
{

// added for Solaris 2.6 
#ifdef no_inet_aton
int inet_aton(const char * addr, in_addr * inp)
{
	// this workaround defines inet_aton in term in_addr. Drawbacks are
	// that there is no useful error checking.
	inp->s_addr = inet_addr(addr);
	return (inp->s_addr != 0);
};
#endif // no_inet_aton

// added for solaris 2.6
#ifdef no_socklen_t
typedef int socklen_t;
#endif // no_sock_len_t

int tcp_socket::transmit(const string & data)
{
	return send(mysock,data.c_str(),data.length(),MSG_DONTWAIT);
};

int tcp_socket::receive(std::string & data, int limit)
{
	errno = 0;
	data = "";
	// make more room in the input buffer if needed.
	int bufsize = limit + 1;
	if (bufsize > inbuff.size()) inbuff.resize(bufsize);
	// get the data to inbuff
	int returnme = recv(mysock,(void *) &(inbuff[0]),limit,0);
	// did we get anything? 
	if (returnme > 0)
	{
		// yes.. store for the caller.
		data.resize(returnme,0);
		for (int i=0; i<returnme; i++)
		{
		  data[i] = inbuff[i];
		};
	}
	// ... or was there an error?
	else if (errno) 
	{
		// flag it for the caller.
		returnme = -1;
	};
	return returnme;
};

tcp_socket::tcp_socket(bool autoclose)
{
	close_on_destruct = autoclose;
	mysock = socket(AF_INET,SOCK_STREAM,0);
	_status = sock_init;
	_last_error = 0;
};

tcp_socket::tcp_socket(int existing_socket, bool autoclose)
{
	close_on_destruct = autoclose;
	mysock = existing_socket;
	_status = sock_connect;
	_last_error = 0;
};

tcp_socket::~tcp_socket()
{	
	if (close_on_destruct)
	{
		shutdown(mysock,SHUT_RDWR);
		::close(mysock);
		_status = sock_undef;
	};
};

void tcp_socket::reset()
{
	// close the old socket if we can.
	try { close(); } catch (tcp_socket::general_exception) {};
	// get a new one.
	mysock = socket(AF_INET,SOCK_STREAM,0);
	// set up the default conditions;
	_status = sock_init;
	_last_error = 0;
};

sockaddr_in tcp_socket::str_to_sockaddr(const string & address)
{
	sockaddr_in in_address;
	// init our address structure.
	memset(&in_address,0,sizeof(in_address));
	//in_address.sin_len = 16;
	in_address.sin_family = AF_INET;
	// seperate the IP and port addresses.
	const int IP = 0; 
	const int PORT = 1;
	strlist addr;
	addr = split(address,':');
	if (addr.size() != 2)
	{
		throw bad_address_exception();
	};
	if (addr[IP] != "*")
	{
		// first attempt name resolution
		hostent * name = gethostbyname(addr[IP].c_str());
		if ((name != 0) && (name->h_length > 0))
		{
			in_address.sin_addr = *( (in_addr *) (name->h_addr_list[0]));
		}
		else if (!inet_aton(addr[IP].c_str(),&in_address.sin_addr))
		{
			throw bad_address_exception();
		};
	};
	if (addr[PORT] != "*")
	{
		// get the good port;
		uint16_t port = lexical_cast<uint16_t>(addr[PORT]);
		in_address.sin_port = htons(port);
	};
	return in_address;
};

string tcp_socket::sockaddr_to_str(const sockaddr_in & address)
{
	// get the IP address.
	string returnme = inet_ntoa(address.sin_addr);
	// adjust for wild card if appropriate.
	if (returnme == "0.0.0.0") { returnme = "*"; };
	// get the port address
	returnme += ":";
	uint16_t myport = ntohs(address.sin_port);
	if (myport > 0 )
	{
		returnme += lexical_cast<string>(myport);
	}
	else
	{
		returnme += "*";
	};
	return returnme;
};

void tcp_socket::bind(const string & address)
{
	if (address != "*:*")
	{
		sockaddr_in in_address = str_to_sockaddr(address);
		socklen_t socklen = sizeof(in_address);
		// try bind.
		if (::bind(mysock, (sockaddr*) &in_address, socklen))
		{
			// failed in some way.
			_last_error = errno;
			cant_bind_exception e;
			e.store(address);
			throw e;
		};
	}; // nop if "*:*" is passed in.
};

void tcp_socket::connect(const string & address, int timeout)
{
	sockaddr_in in_address = str_to_sockaddr(address);
	if (::connect(mysock, ( sockaddr *) &in_address, sizeof(in_address)))
	{
		_last_error = errno;
		bad_connect_exception e;
		e.store(address);
		throw e;
	}
	else
	{
		_status = sock_connect;
	};
};

void tcp_socket::close()
{
	if (::close(mysock))
	{
		_last_error = errno;
		throw close_exception();
	}
	else 
	{
		_status = sock_close;
	};
};

int tcp_socket::put(string s, int timeout)
{
	// set up for the testing loop.
	time_t endtime = time(NULL);
	int orig_len = s.length();
	endtime += timeout;
	// Try to send.
	bool done = false;
	while (!done)
	{
		// make an attempt.
		int results = transmit(s);
		// remove the chars already sent from the buffer and set done 
		// if appropriate.
		if (results > 0) 
		{ 
			s.erase(0,results);
			done = (s.length() == 0) ? true : false;
		};
		// decide what do do about any possible error results.
		if (results == -1)
		{
			switch (errno)
			{
				case EBADF :
				case ENOTSOCK: 
					{
						done = true; 
						_last_error = errno;
						_status = sock_close;
						not_open_exception e;
						e.store(s);
						throw e;
					 	break;
					};
				case ENOBUFS :
				case ENOMEM  :
					{
						done = true;
						_last_error = errno;
						buffer_full_exception e;
						e.store(s);
						throw e;
						break;
					};
				case EINTR:
				case EAGAIN :
//				case EWOULDBLOCK :
					{
						usleep(50);
						break;
					};
				default : 
					{
						done = true;
						_last_error = errno;
						general_exception e;
						e.store(s);
						throw e;
						break;
					};
			}; // switch (errno)
		}; // until error, 
		if ((timeout > 0) && ((time(NULL) >= endtime)))
		{
			done =  true;
		};
	}; // continue sending until success or error or timeout.
	// check for timeout.
	int sent = orig_len - s.length();
	if (sent != orig_len)
	{
		timeout_exception e;
		e.store(s);
		throw e;
	};
	return sent;
};

string tcp_socket::get(int maxlen, int timeout)
{
/*
micro_timer run_time;
run_time.start();
std::cerr << "ENTER get(" << maxlen << "," << timeout << ")" << std::endl;
*/
	string returnme = "";
	returnme.reserve(maxlen);
	string in_buffer;
	// set out timeout marker.
	time_t endtime = time(NULL);
	endtime += timeout; 
	timeval wait;
	// input loop
	bool done = false;
	while (!done)
	{
		time_t waittime = endtime - time(NULL);
		wait.tv_sec = (waittime > 0) ? waittime : 0;
		wait.tv_usec = 10;
		// set max timeout. 
		//  the assert is used because some platforms may not properly support
		//  the SO_RCVTIMEO socket option.
		setsockopt(mysock,SOL_SOCKET,SO_RCVTIMEO,&wait,sizeof(wait));
		// is there any data waiting?
		int results = receive(in_buffer,maxlen-returnme.size());
		// was there an error?
		switch (results)
		{
			case 0:
			{
				if (returnme == "") 
				{
					throw not_open_exception();
				}
				else
				{
					done = true;
				}
				break;
			}
			case -1 :
			{
				switch (errno)
				{
					case EBADF :
					case ENOTCONN :
					case ENOTSOCK :
					{
						done = true;
						_last_error = errno;
						_status = sock_close;
						if (returnme == "")
						{
							not_open_exception e;
							e.store(returnme);
							throw e;
						};
						break;
					}
					case EAGAIN :      // shouldn't happen, but .....
					case EINTR :       // this too....
					{
						if (timeout == 0)
						{
							done = true;
						};
						usleep(50);
						break;
					}
					default :
					{
						done = true;
						_last_error = errno;
						if (returnme == "")
						{
							general_exception e;
							e.store(returnme);
							throw e;
						};
						break;
					}
				}; // there was an error.
				break;
			}
			default: 
			{
				returnme += in_buffer;
				break;
			}
		};
		// check boundry conditions;
		// -- maxlen only effective if maxlen > 0.
		if ((maxlen > 0) && (returnme.length() >= maxlen)) { done = true; };
		// -- timeout only effective if timeout > 0.
		if ((timeout > 0) && (time(NULL) > endtime))      { done = true; };
		if (timeout == 0) { done = true; };
/*
std::cerr << "\tstatus: " << returnme.size() 
<< ", done = " << done 
<< " (" << run_time.interval() << ")"	
<< std::endl;
*/
	}; // input loop.
	// did we time out?
	if ((timeout > 0) && (time(NULL) > endtime))
	{
		timeout_exception e;
		e.store(returnme);
		throw e;
	};
	// done! 
	return returnme;
};


string tcp_socket::getln(string eol, int maxlen, int timeout)
{
	// args check.
	if (eol == "")
	{
		throw bad_args_exception();
		return eol;
	};
	// setup.
	string returnme = "";
	returnme.reserve(maxlen);
	int targetlen = eol.length();
	int searchlen = targetlen - 1;
	time_t endtime = time(NULL);
	endtime += timeout;
	bool done = false;
	bool success = false;
	// first, get at least as many characters as are in the eol string.
	try
	{
		int wait_time = timeout;
		if (timeout < 0) { wait_time = 0; };	
		returnme = get(targetlen,wait_time);
	}
	catch (timeout_exception e)
	{	
		// if get timed out we may be done.
		if (timeout >= 0) { done = true; };
		returnme = e.comment();
	};
	// We only need to continue if we have not gotten our token yet.
	if ((!done) && (returnme.length() >= targetlen))
	{
//		success = returnme.substr(returnme.length()-targetlen,targetlen) == eol;
		success = returnme.find(eol,returnme.length()-targetlen) != string::npos;
	};
	while (!done && !success)
	{
		// still more to get, if we have time.
		int timeleft = endtime - time(NULL);
		timeleft = (timeleft > 0) ? timeleft : 0;
		// can we increase our get length?
		int getlen = 1;
		if (searchlen > 1)
		{
			// see if a token start is near.
			if (returnme.find(eol[0],returnme.length()-searchlen) == string::npos)
			{
				// great!.. get just less than one token's worth.
				getlen = searchlen;
			}
		};
		// okay.. let's get the next chunk
		try
		{
			returnme += get(getlen,timeleft);
		}
		catch (timeout_exception e)
		{
			if (timeout >= 0) { done = true; };
		};
		// check boundary conditions.
		// -- did we get it?
		if (returnme.length() >= targetlen)
		{
//			success = returnme.substr(returnme.length()-targetlen,targetlen) == eol;
			success = returnme.find(eol,returnme.length()-targetlen) != string::npos;
		};
		// -- maxlen only effective if maxlen > 0.
		if ((maxlen > 0) && (returnme.length() >= maxlen)) { done = true; };
	};
	// did we error out?
	if (!success)
	{
		// what type of error was it?
		if ((maxlen > 0) && (returnme.length() >= maxlen))
		{
			overrun_exception e;
			e.store(returnme);
			throw e;
		}
		else
		{
			timeout_exception e;
			e.store(returnme);
			throw e;
		};
	};
	return returnme;
};

string tcp_socket::get_local_address()
{	
	sockaddr_in myaddr;
	socklen_t len = sizeof(myaddr);
	int results = getsockname(mysock,(sockaddr *) &myaddr,&len);
	// check that getsockname was okay.
	if (results == -1)
	{
		// some type of error occurred.
		switch (errno)
		{
			case ENOBUFS :
				{
					throw buffer_full_exception();
					break;
				};
			default :
				{
					throw general_exception();
					break;
				};
		}; // switch errno
	}
	return sockaddr_to_str(myaddr);
};

string tcp_socket::get_remote_address()
{
	sockaddr_in myaddr;
	socklen_t len = sizeof(myaddr);
	int results = getpeername(mysock,(sockaddr *) &myaddr,&len);
	// check that getsockname was okay.
	if (results == -1)
	{
		// some type of error occurred.
		switch (errno)
		{
			case ENOTCONN :
				{
					_status = sock_close;
					throw not_open_exception();
					break;
				};
			case ENOBUFS :
				{
					throw buffer_full_exception();
					break;
				};
			default :
				{
					throw general_exception();
					break;
				};
		}; // switch errno
	}
	return sockaddr_to_str(myaddr);
};


tcp_server_socket_factory::tcp_server_socket_factory(
		const string & address, const unsigned short int & backlog)
{
	// does not attempt to set up the connection initially.
	_address = address;
	_backlog = backlog;
	_last_thread_fault = 0;
	thread_return = 0;
	in_on_accept = false;
	connect_sock = 0;
};

tcp_server_socket_factory::~tcp_server_socket_factory()
{
	// are we current handling requests?
	if (is_running())
	{
		// yes, this is -rude- shutdown of the listening thread.
		// Get over it.
		thread_data.lock();
		okay_to_continue = false;
		thread_data.unlock();
		close(listen_sock);
		stop();
	};
};

void tcp_server_socket_factory::start_listen()
{
	// take no action if the listen thread is already running.
	if (!is_running())
	{
		// load up the control flags
		in_on_accept = false;
		okay_to_continue = true;
		thread_return = 0;
		// start it up!
		detach();
		start();
	};
};

void tcp_server_socket_factory::stop_listen()
{
	// take action only if the listen thread is running.
	if (is_running())
	{
		bool okay_to_kill = false;
		// set the flag that indicates the thread should not process any more.
		{
			scope_lock lock(thread_data);
			bool okay_to_kill = !in_on_accept;
		}
		// close the listener socket.
		close(listen_sock);
		// is is okay to cancel the thread?
		if (!okay_to_kill)
		{
			// we'll allow up to 30 seconds for on_accept() to finish.
			time_t endtime = time(NULL);
			endtime += 30;
			bool done = false;
			while (!done)
			{
				// wait a second...
				sleep(1);
				// check boundaries.
				// -- are we out of on_accept()?
				{
				  scope_lock lock(thread_data);
				  done = !in_on_accept;
				}
				// -- are we out of time?
				okay_to_kill = done;
				if (time(NULL) > endtime) { done = true; };
			};
		};
		// kill the listener thread.
		if (!okay_to_kill) { throw on_accept_bound_exception(); };
		stop();
	};
};

bool tcp_server_socket_factory::listening()
{
	bool running = is_running();
	if (!running)
	{
		// check to be sure the thread did not die due to an error.
		if (thread_return != 0)
		{
			// Clear the error state before throwing an exception.
			_last_thread_fault = thread_return;
			thread_return = 0;
			// if thread_return was non-zero, it is assumed the thread died an
			// evil and useless death. Scream in anger!
			throw listen_terminated_exception();
		};
	};
	return running;
};

unsigned short int tcp_server_socket_factory::backlog()
{
	return _backlog;
};

void tcp_server_socket_factory::run()
{
	/* Put this entire thing in a try block to protect the application. 
	 * Without this, an untrapped exception thrown here or in the user supplied
	 * on_accept() method would abort the entire application instead of just this
	 * thread.
	 */
	try
	{
		bool go = true;
		// set up our listening socket.
		listen_sock = socket(AF_INET,SOCK_STREAM,0);
		sockaddr_in myaddr;
		try
		{
			myaddr = tcp_socket::str_to_sockaddr(_address);
		}
		catch (...)
		{
			// probably a tcp_socket::bad_address_exception, 
			// but any reason will do.
			go = false;
		};
		if (bind(listen_sock,(sockaddr *) &myaddr,sizeof(myaddr)))
		{
			// bind did not work.
			go = false;
		};	
		if (listen(listen_sock,_backlog))
		{
			// listen failed in some way.. I don't care which.
			go = false;
		};
		// processing loop
		while (go)
		{
			// are we okay to proceed?
			{
			  scope_lock lock(thread_data);
			  in_on_accept = false;
			  go = okay_to_continue;
			}
			if (!go)
			{
				close(listen_sock);
				exit(0);
			};
			// accept a new connection
			bool good_connect = true;
			int new_conn = accept(listen_sock,NULL,NULL);
			// validate the accept return value.
			if (new_conn == -1)
			{
				// accept returned an error.
				switch (errno) 
				{
					case ENETDOWN :
					case EPROTO :
					case ENOPROTOOPT :
					case EHOSTDOWN :
					case ENONET :
					case EHOSTUNREACH :
					case EOPNOTSUPP :
					case ENETUNREACH :
					case EAGAIN :
//					case EWOULDBLOCK : 
					case EPERM :
					case ECONNABORTED :
						{
							good_connect = false;
							break;
						};
					default : 
						{	
							// for any other error, we're going to shutdown the 
							// this listener thread.
							{
							  scope_lock lock(thread_data);
							  // If the error was caused by an attempt to close the 
							  // socket and shutdown the thread, don't store an
							  // error flag.
							  if (okay_to_continue)
							  {
								  thread_return = errno;
							  }
							  else
							  {
								  thread_return = 0;
							  };
							};
							exit(errno);
							break;
						};
				};  // switch (errno)
			}; // error thrown by accept.
			// create connect_sock
			connect_sock = new tcp_socket(new_conn);
			// are we okay to proceed?
			{
			  scope_lock lock(thread_data);
			  go = okay_to_continue;
			  // if we are go, then we'll be going to on_accept next.
			  // Therefore, this equality makes sense if you hold your
			  // head just so. :-P
			  in_on_accept = go;
			}
			if (!go)
			{
				delete connect_sock;
				close(listen_sock);
				exit(0);
			};
			// only call on_accept() if we have a good connection.
			if (good_connect)
			{
				// make the thread easily cancelable.
				set_cancel_anytime();
				// call on_accept
				on_accept();
				// set back to cancel_deferred.
				set_deferred_cancel();
			};
		}; // while go;
		// if we get here then things are not going well...
		{
		  scope_lock lock(thread_data);
		  thread_return = -2;
		}
		close(listen_sock);
		exit(-2);
	}
	catch (...)
	{
		/* an untrapped exception was thrown by someone in this thread.
		 * We'll shutdown this thread and put -1 in the thread_return field
		 * to let the world know that we don't know what killed us.
		 */
		thread_data.lock();
		thread_return = -1;
		thread_data.unlock();
		close(listen_sock);
		exit(-1);
	};
};

} // namespace nrtb




























