/*********************
Multithreaded message passing example in D
*********************/

// import the concurrency and standard IO modules
import std.concurrency, std.stdio;

// main is just like main in C++
void main()
{
  // intialize some variable; the compiler can figure out the type from usage.
  auto low=0, high = 100;

  // launch a second thread using function receiver defined below.
  auto rtid = spawn(&receiver);

  // foreach iterates a range; in this case all values between
  // low and high are assigned are assigned to i sequencially.
  foreach (i; low .. high)
  {
    writeln("Sent ", i);
    // sends a message to receiver using it's thread id; queue management is automatic.
    rtid.send(i);
  }
  writeln("** Main is complete **");
}

// receiver is a function which is run as a seperate thread by main.
void receiver()
{
  // a variable for error checking.
  auto lastid = -1;

  // function to handle messages
  void int_handler(int i)
  {
    // just print a line and record the last id received
    writeln(" Received ",i);
    lastid = i;
  }

  // Loop until we the parent shuts down.
  bool running = true;
  while (running)
  {
    receive
    (
      // handler for int messages
      (int msg) { int_handler(msg); },
      // handler for "owner terminated" exception
      (OwnerTerminated e) { running = false; }
    );
  }

  // quick check to be sure we got them all.
  assert(lastid == 99);
  writeln("** Normal Shutdown **");
}