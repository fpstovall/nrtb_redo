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

#ifndef nrtb_circular_queue_h
#define nrtb_circular_queue_h

#include <iostream>
#include <boost/circular_buffer.hpp>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace nrtb
{

/********************************************************
 * The circular_queue template is designed for use with
 * the classic producer/consumer thread management model.
 * The producer uses circular_queue::push() to put items
 * in the queue as they become available, and the consumer
 * thread calls circular_queue::pop() when it is ready
 * for the next item to work.
 *
 * Common uses would be for buffering outgoing or incomming
 * messages from a communications channel, providing a feed
 * queue for parallel threads to make full use of multi-core
 * processors, or any case where one or more threads are
 * passing data to another set of threads.
********************************************************/
template <class T>
class circular_queue
{
public:
  class queue_not_ready: public std::exception {};
  std::atomic<int> in_count {0};
  std::atomic<int> out_count {0};

  /*********************************************
    * creates the queue with the specified
    * number of elements. All memory is allocated
    * at construction to minimize delays at runtime.
  *********************************************/
  circular_queue(int size);

  /*********************************************
    * releases all items in the queue
  *********************************************/
  virtual ~circular_queue();

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
  // resizes the buffer, may cause data loss
  void resize(int newsize);
  // clears the buffer, data will be discarded.
  void clear();

protected:

  boost::circular_buffer<T> buffer;
  std::condition_variable signal;
  std::mutex mylock;
  bool ready {true};
};

template <class T>
circular_queue<T>::circular_queue(int size)
{
  buffer.set_capacity(size);
};

template <class T>
circular_queue<T>::~circular_queue()
{
  shutdown();
};

template <class T>
void circular_queue<T>::push(T item)
{
  if (ready)
  {
    in_count++;
    {
      std::unique_lock<std::mutex> lock(mylock);
      buffer.push_back(item);
    }
    signal.notify_one();
  }
  else 
  {
    queue_not_ready e;
    throw e;
  }
};

template <class T>
T circular_queue<T>::pop()
{
  std::unique_lock<std::mutex> lock(mylock);
  while (buffer.empty() && ready)
      signal.wait(lock);
  if (ready)
  {
    T returnme = buffer.front();
    buffer.pop_front();
    out_count++;
    return returnme;    
  }
  else
  {
    queue_not_ready e;
    throw e;
  };
};

template <class T>
void circular_queue<T>::shutdown()
{
  try
  {
    std::unique_lock<std::mutex> lock(mylock);
    ready = false;
    buffer.clear();
    signal.notify_all();
  }
  catch (...) {}  
}

template <class T>
int circular_queue<T>::size()
{
  std::unique_lock<std::mutex> lock(mylock);
  return buffer.size();
};

template <class T>
void circular_queue<T>::resize(int newsize)
{
  std::unique_lock<std::mutex> lock(mylock);
  buffer.set_capacity(newsize);
};

template <class T>
void circular_queue<T>::clear()
{
  std::unique_lock<std::mutex> lock(mylock);
  buffer.clear();
};

} // namespace nrtb

#endif //nrtb_circular_queue_h//
