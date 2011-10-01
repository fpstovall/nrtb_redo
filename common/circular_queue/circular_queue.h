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

#include <base_thread.h>
#include <boost/circular_buffer.hpp>

namespace nrtb
{

/********************************************************
 * The circular_queue template is designed for use with 
 * the classic producer/consumer thread management model. 
 * The producer uses circular_queue::push() to put items
 * in the queue as they become available, and the consumer
 * thread calls circular_queue::park() when it is ready 
 * for the next item to work.
 * 
 * Common uses would be for buffering outgoing or incomming
 * messages from a communications channel, providing a feed
 * queue for parallel threads to make full use of multi-core 
 * processors, or any case where one or more threads are 
 * passing data to another set of threads.
********************************************************/
template <class T>
class cirular_queue
{
public:
  
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
  
  int size();
  void resize(int newsize);
  void clear();
  
protected:
  
  boost::circular_buffer<T> buffer;
  cond_variable buffer_lock;
};
  
template <class T>
cirular_queue::circular_queue<T>(int size)
{
  buffer.resize(int);
};

template <class T>
cirular_queue<T>::~circular_queue()
{
  scope_lock lock(buffer_lock);
  buffer.clear();
};

template <class T>
void cirular_queue<T>::push(T item)
{
  scope_lock lock(buffer_lock);
  buffer.push_back(item);
  buffer_lock.signal();
};

template <class T>
T cirular_queue<T>::pop()
{
  scope_lock lock(buffer_lock);
  while (buffer.empty())
	buffer_lock.wait();
  T returnme = buffer.front();
  buffer.pop_front();
  return returnme;
};

template <class T>
int cirular_queue<T>::size()
{
  scope_lock lock(buffer_lock);
  return buffer_lock.size();
};

template <class T>
void cirular_queue<T>::resize(int newsize)
{
  scope_lock lock(buffer_lock);
  buffer.resize(newsize);
};

template <class T>
void cirular_queue<T>::clear()
{
  scope_lock lock(buffer_lock);
  buffer.clear();
};


} // namespace nrtb
 
#endif //nrtb_circular_queue_h//