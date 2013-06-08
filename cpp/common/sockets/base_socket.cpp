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
#include <boost/lexical_cast.hpp>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>

// testing
#include <iostream>

using std::lexical_cast;
using std::string;

namespace nrtb
{

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
  _status = state::sock_init;
  _last_error = 0;
};

tcp_socket::tcp_socket(int existing_socket, bool autoclose)
{
  close_on_destruct = autoclose;
  mysock = existing_socket;
  _status = state::sock_connect;
  _last_error = 0;
};

tcp_socket::~tcp_socket()
{	
  if (close_on_destruct)
  {
    try {shutdown(mysock,SHUT_RDWR); } catch (...) {};
    try {::close(mysock); } catch (...) {};
    _status = state::sock_undef;
  };
};

void tcp_socket::reset()
{
  // close the old socket if we can.
  try { close(); } catch (tcp_socket::general_exception) {}
  // get a new one.
  mysock = socket(AF_INET,SOCK_STREAM,0);
  // set up the default conditions;
  _status = state::sock_init;
  _last_error = 0;
};

TODO: Need to continue cleanup from here.

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
		const string & address, const unsigned short int & backlog = 5)
{
  // does not attempt to set up the connection initially.
  _address = address;
  _backlog = backlog;
  _last_thread_fault = 0;
};

tcp_server_socket_factory::~tcp_server_socket_factory()
{
  // make sure we've stopped doing anything.
  try { stop_listen(); } catch (...) {};
};

void tcp_server_socket_factory::start_listen()
{
  // take no action if the listen thread is already running.
  if (!is_running())
  {
    // start it up!
    start();
    int countdown = 99;
    while (!listening() and countdown)
    {
      countdown--;
      usleep(1e3);
    }
    if (!countdown)
    {
      bind_failure_exception e;
      e.store(_address);
      throw e;
    };
  }
  else
  {
    throw already_running_exception();
  };
};

void tcp_server_socket_factory::stop_listen()
{
  // take action only if the listen thread is running.
  if (listening())
  {
    // stop the listener thread
    stop();
    join();
    listen_sock.reset();
  };
};

bool tcp_server_socket_factory::listening()
{
  bool running = is_running();
  if (listen_sock)
  {
    running = running
      and listen_sock->status() == tcp_socket::sock_connect;
  }
  else
  {
    running = false;
  };
  return running;
};

int tcp_server_socket_factory::last_fault()
{
  return _last_thread_fault;
};

unsigned short int tcp_server_socket_factory::backlog()
{
	return _backlog;
};

void tcp_server_socket_factory::run()
{
  // set up the listening socket.
  int sock;
  _last_thread_fault = 0;
  bool go = false;
  try
  {
    sock = socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in myaddr;
    myaddr = tcp_socket::str_to_sockaddr(_address);
    int a = bind(sock,(sockaddr *) &myaddr,sizeof(myaddr));
    int b = listen(sock,_backlog);
    if (a || b)
    {
      go = false;
      if (a) _last_thread_fault += 1;
      if (b) _last_thread_fault += 2;
    }
    else
    {
      go = true;
    };
  }
  catch (...)
  {
    _last_thread_fault = 100;
  };
  // if not in a good state, terminate the thread.
  if (!go)
  {
    _last_thread_fault =+ 200;
    exit(0);
  };
  // make sure the listener is closed when we exit.
  // also prevides an external hook to socket.
  listen_sock.reset(new tcp_socket(sock));
  // processing loop
  while
  (
    go
    and listen_sock
    and (listen_sock->status() == tcp_socket::sock_connect)
  )
  {
    // accept a new connection
    bool good_connect = true;
    int new_conn = accept(sock,NULL,NULL);
    // is the listener still open?
    if
    (
      (!listen_sock)
      and (listen_sock->status() != tcp_socket::sock_connect)
    )
    {
      // the listner socket is not available.. get out of here.
      listen_sock.reset();
      _last_thread_fault = 300;
      exit(0);
    };
    // validate the accept return value.
    if (new_conn == -1)
    {
      // accept returned an error.
      switch (errno)
      {
        case EPROTO :
        case EHOSTDOWN :
        case EHOSTUNREACH :
        case EAGAIN :
        case ECONNABORTED :
        {
          // abandon this connection
          good_connect = false;
          break;
        };
        default :
        {
          // for any other error, we're going to shutdown the
          // this listener thread.
          go = false;
          good_connect = false;
          _last_thread_fault = errno;
          break;
        };
      };  // switch (errno)
    }; // error thrown by accept.
    if (good_connect)
    {
      connect_sock.reset(new tcp_socket(new_conn));
      set_cancel_anytime();
      // call the connection handler.
      try
      {
        go = on_accept();
      }
      catch (...)
      {
        go = false;
        _last_thread_fault = 501;
      };
      set_deferred_cancel();
      // safety check.
      if (connect_sock)
      {
        std::cerr << "WARNING: on_accept() did not take ownership of "
          << "connect_sock.\n"
          << "  This can lead to leaks and should be fixed."
          << std::endl;
        connect_sock.reset();
        _last_thread_fault = 500;
        go = false;
      };
    };
  }; // while go;
};

} // namespace nrtb

