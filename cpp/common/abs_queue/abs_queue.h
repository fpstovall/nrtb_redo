/***********************************************
 This file is part of the NRTB project (https://github.com/fpstovall/nrtb_beta).

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

#ifndef nrtb_abs_queue_h
#define nrtb_abs_queue_h

#include <common.h>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

namespace nrtb
{

/********************************************************
 * The abs_queue template is designed for use with
 * the classic producer/consumer thread management model.
 * The producer uses abs_queue::push() to put items
 * in the queue as they become available, and the consumer
 * thread calls abs_queue::park() when it is ready
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
template <class T, class queue_t>
class abs_queue
{
public:
  class queue_not_ready: public nrtb::base_exception {};
    
    /// Total number of items placed in queue.
    std::atomic<int> in_count {0};
    /// Total number of items read from queue.
    std::atomic<int> out_count {0};
    /// Total number reported done by workers
    std::atomic<int> completed {0};

    /*********************************************
      * creates the queue with the specified
      * number of elements. 
    *********************************************/
    abs_queue();

    /*********************************************
      * releases all items in the queue
    *********************************************/
    ~abs_queue();

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
      * blocks queue processing temporarily.
    *********************************************/
    void hold();

    /*********************************************
      * Releases the queue if held.
    *********************************************/
    void release();

    /*********************************************
      * Returns true if the queue is blocked 
      *  false otherwise.
    *********************************************/
    bool is_blocked();

    /*********************************************
     * puts the queue in shutdown mode.
    *********************************************/
    void shutdown();

    /*********************************************
     * Blocks until the queue is empty. This is 
     * only safe to use if the workers all call 
     * task_done() at the end of processing for 
     * each packet. Using join() with workers 
     * which do not properly call task_done() 
     * is an error which will hang your code.
    *********************************************/
    void join();

    /********************************************
     * Called by workers when they complete work
     * on each packat. This is required if join()
     * is to be used to wait for the queue to
     * drain. If not used, a call to join() will
     * block forever. 
    ********************************************/
    void task_done();

    // returns the number of items in the queue
    int size();
    // resizes the buffer, may cause data loss
    void resize(int newsize);
    // clears the buffer, data will be discarded.
    void clear();

protected:

    queue_t buffer;
    std::mutex mylock;
    std::condition_variable signal;
    std::atomic<bool> ready {true};
    std::atomic<bool> blocked {false};
};

template <class T, class queue_t>
abs_queue<T,queue_t>::abs_queue()
{
};

template <class T, class queue_t>
abs_queue<T,queue_t>::~abs_queue()
{
  shutdown();
};

template <class T, class queue_t>
void abs_queue<T,queue_t>::push(T item)
{
  if (ready)
  {
    in_count++;
    {
      std::unique_lock<std::mutex> lock(mylock);
      buffer.push(std::move(item));
    }
    if (!blocked) signal.notify_one();
  }
  else 
  {
    queue_not_ready e;
    throw e;
  }
};

template <class T, class queue_t>
T abs_queue<T,queue_t>::pop()
{
  std::unique_lock<std::mutex> lock(mylock);
  while ((buffer.empty() && ready) or (blocked && ready))
    signal.wait(lock);
  if (ready)
  {
    T returnme = std::move(buffer.front());
    buffer.pop();
    out_count++;
    return std::move(returnme);
  }
  else
  {
    queue_not_ready e;
    throw e;
  };
};

template <class T, class queue_t>
void abs_queue<T,queue_t>::hold() 
{
  blocked = true;
};

template <class T, class queue_t>
void abs_queue<T,queue_t>::release()
{
  blocked = false;
  signal.notify_all();
};

template <class T, class queue_t>
bool abs_queue<T,queue_t>::is_blocked()
{
  return blocked;
};

template <class T, class queue_t>
void abs_queue<T,queue_t>::shutdown()
{
  try
  {
    std::unique_lock<std::mutex> lock(mylock);
    ready = false;
    while (buffer.size()) { buffer.pop(); }
    signal.notify_all();
  }
  catch (...) {}  
}

template <class T, class queue_t>
void abs_queue<T,queue_t>::join()
{
  while (completed != in_count) usleep(100);
}

template <class T, class queue_t>
void abs_queue<T,queue_t>::task_done()
{
  completed++;
}

template <class T, class queue_t>
int abs_queue<T,queue_t>::size()
{
  std::unique_lock<std::mutex> lock(mylock);
  return buffer.size();
};

template <class T, class queue_t>
void abs_queue<T,queue_t>::resize(int newsize)
{
  std::unique_lock<std::mutex> lock(mylock);
  buffer.set_capacity(newsize);
};

template <class T, class queue_t>
void abs_queue<T,queue_t>::clear()
{
  std::unique_lock<std::mutex> lock(mylock);
  while (buffer.size()) buffer.pop();
};

} // namespace nrtb

#endif //nrtb_abs_queue_h//
