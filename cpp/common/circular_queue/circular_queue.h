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
#include <common.h>
#include <abs_queue.h>

namespace nrtb
{
  
// specialize boost::circular_buffer to act like a queue.
template <class T> 
class base_cq : public boost::circular_buffer<T>
{
public:
  void push(T elem) { this->push_back(std::move(elem)); };
  void pop() { this->pop_front(); };
};

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
class circular_queue : public abs_queue<T, base_cq<T>> 
{
public:  
  // convienence constructor because a 0 size circular_queue is useless.
  circular_queue(int size=10) : abs_queue<T, base_cq<T>>() 
  {
    this->resize(size);
  };
};

} // namespace nrtb

#endif //nrtb_circular_queue_h//
