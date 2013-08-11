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

/* implements a template that takes any class provided and 
 * creates a singleton for it.
 * 
 * fps 2002-5-29
 */

#ifndef nrtb_singleton_h
#define nrtb_singleton_h

#include <mutex>
#include <memory>
#include <atomic>

namespace nrtb
{

/** Wrapper template to create singleton classes.
 ** 
 ** Classic Singleton pattern template, with thread-safe
 ** methods for getting an instance and destruction.
 ** 
 ** Singletons are classes that:
 ** (1) Insure that there are no more than one instance of the 
 ** class at any time.
 ** (2) Allocate themselves automagically when accessed for the 
 ** first time.
 ** (3) Provide a globally available access method to allow any
 ** scope to gain access and use the instance.
 ** 
 ** This template can create a singleton version of any class that has a
 ** no-argument constructor. The resulting class will have the same
 ** inteface as the object used to create the template with the constructor
 ** and destructor made protected and the addition of the get_instance()
 ** and delete_me() methods.
 ** 
 ** See the documentation on the get_instance() and delete_me() methods for
 ** usage.
 **/
template <class T, int mytag=0>
class singleton: public T
{
private:
  static std::mutex __mylock;
  static singleton<T,mytag> * __me ;
protected:
  singleton() : T() {};
  virtual ~singleton() {};
  singleton(const singleton &) = delete;
public:
  /** Used to access the object.
    ** 
    ** Returns a reference to the instanciated singleton object. If
    ** the object has not been accessed before, the object will be 
    ** instanciated automatically.
    ** 
    ** As is usual for the method that provides access to a singleton, 
    ** this method is static, allowing it to be called via the class 
    ** name as shown. Remember you can only assign the return value to
    ** a reference:
    ** 
    ** 	mytype & a = mytype::get_instance();
    ** 
    ** Attempts to make a copy of the instance should throw a compiler
    ** error.
    ** 
    ** This method is thread safe, but that does not imply that the 
    ** class returned is thread-safe overall; that would depend on the 
    ** implementation of the class used to instaciate the template.
    **/
  static singleton & get_reference()
  {
    // First test avoids expensive mutex cycle
    // if the object is already allocated.
    if (!__me)
    {
      std::unique_lock<std::mutex> lock(__mylock);
      // second test required in case multiple threads
      // get past the first check.
      if (!__me) 
      { 
	__me = new singleton;
      };
    };
    return *__me;
  };

  /** Destructs and deallocates the singleton object.
    ** 
    ** After a call to this method, the singleton object will
    ** be destructed and deallocated from memory. However, it
    ** will be automatically reconstruted and allocated if
    ** get_instance() called at any time afterword.
    **/
  void delete_me()
  {
    std::unique_lock<std::mutex> lock(__mylock);
    if (__me) delete __me;
    __me = 0;
  };
};

template <class T, int mytag> 
  std::mutex singleton<T,mytag>::__mylock;
template <class T, int mytag> 
  singleton<T,mytag> * singleton<T,mytag>::__me; 

} // namespace nrtb;

#endif // nrtb_singleton_h
