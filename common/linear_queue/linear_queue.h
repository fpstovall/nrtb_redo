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

#ifndef nrtb_linear_queue_h
#define nrtb_linear_queue_h

#include <iostream>
#include <base_thread.h>
#include <list>

namespace nrtb
{

/********************************************************
 * The linear_queue template is designed for use with
 * the classic producer/consumer thread management model.
 * The producer uses linear_queue::push() to put items
 * in the queue as they become available, and the consumer
 * thread calls linear_queue::park() when it is ready
 * for the next item to work.
 * 
 * This queue will expand as needed. Constrast this with
 * the circular_queue, which is of a fixed size and will 
 * drop older data when full
 *
 * Common uses would be for buffering outgoing or incomming
 * messages from a communications channel, providing a feed
 * queue for parallel threads to make full use of multi-core
 * processors, or any case where one or more threads are
 * passing data to another set of threads.
********************************************************/
template <class T>
class linear_queue
{
public:
    class queue_not_ready: public base_exception {};

    /*********************************************
      * creates the queue with the specified
      * number of elements. 
    *********************************************/
    linear_queue();

    /*********************************************
      * releases all items in the queue
    *********************************************/
    virtual ~linear_queue();

    /*********************************************
      * Puts an item in the queue.
    *********************************************/
    void push(T item);

    /*********************************************
      * Pops the next item off the queue, blocking
      * if needed until an item becomes available.
    *********************************************/
    T pop();

    /*********************************************
     * puts the queue in shutdown mode.
    *********************************************/
    void shutdown();

    // returns the number of items in the queue
    int size();
    // clears the buffer, data will be discarded.
    void clear();

protected:

    std::list<T> buffer;
    cond_variable buffer_lock;
    bool ready;
};

template <class T>
linear_queue<T>::linear_queue()
{
    ready = true;
};

template <class T>
linear_queue<T>::~linear_queue()
{
};

template <class T>
void linear_queue<T>::push(T item)
{
  if (ready)
  {
	scope_lock lock(buffer_lock);
    buffer.push_back(item);
    buffer_lock.signal();
  }
  else 
  {
	queue_not_ready e;
	  throw e;
  }
};

template <class T>
T linear_queue<T>::pop()
{
    scope_lock lock(buffer_lock);
    while (buffer.empty() && ready)
        buffer_lock.wait();
    if (!ready)
    {
        queue_not_ready e;
        throw e;
    };
	T returnme = buffer.front();
	buffer.pop_front();
	return returnme;
};

template <class T>
void linear_queue<T>::shutdown()
{
  try
  {
	scope_lock lock(buffer_lock);
	ready = false;
	buffer_lock.broadcast_signal();
	buffer.clear();
  }
  catch (...) {}  
}


template <class T>
int linear_queue<T>::size()
{
    scope_lock lock(buffer_lock);
    return buffer.size();
};

template <class T>
void linear_queue<T>::clear()
{
    scope_lock lock(buffer_lock);
    buffer.clear();
};

} // namespace nrtb

#endif //nrtb_linear_queue_h//
