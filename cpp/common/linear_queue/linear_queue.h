/***********************************************
 This file is part of the NRTB project (https://github.com/fpstovall/nrtb_alpha).

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

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <abs_queue.h>

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
class linear_queue : public abs_queue<T,std::queue<T>> {};

} // namespace nrtb

#endif //nrtb_linear_queue_h//
