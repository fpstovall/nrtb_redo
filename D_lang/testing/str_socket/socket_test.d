/*********************
Multithreaded message passing example in D
*********************/

// import the concurrency and standard IO modules
import std.concurrency, std.stdio;
import std.string, std.conv, std.stream, std.stdio;
import std.socket, std.socketstream;

// main is just like main in C++
void main(string[] args)
{
  writeln("D Socket Send/Receive Test.");

  if (args.length < 2)
  {
    writeln("Usage:");
    writeln("      ",arg[0]," httpd_IP");
    return 0;
  } 

  string IP = args[1];
  int port = 80;
  
  writeln("  Connecting to host %s:%d....",IP,port)
  
  // connect to the server
  Socket sock = new TcpSocket(new InternetAddress(IP, port));
  scope(exit) sock.close();
  Stream ss   = new SocketStream(sock);
  
  ss.writeString("GET / HTTP/1.0\r\nHost: " ~ IP ~ "\r\n\r\n");
  
  while (!ss.eof())
  {
    auto line = ss.readLine();
    writeln(line);
  }
  
  writeln("** main() is complete **");

}

