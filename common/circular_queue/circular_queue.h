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
	 * Puts an item in the queue. If threads are 
	 * parked waiting on items, they'll be released.
	*********************************************/
	void push(T);
	
	/*********************************************
	 * Pops an item off the queue, or throws if 
	 * one is not availale.
	*********************************************/
	T pop();
	
	/*********************************************
	 * Like pop, but instead of throwing blocks 
	 * until an item becomes available. This is 
	 * the prefered method for use when threads
	 * are looking for data to work with.
	*********************************************/
	T park();
	
	int size();
	void resize(int newsize);
	
  protected:
	
	boost::circular_buffer<T>
	cond_variable buffer_lock;
  };
  
} // namespace nrtb
 
#endif //nrtb_circular_queue_h//