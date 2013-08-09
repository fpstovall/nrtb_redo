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

#ifndef nrtb_confreader_h
#define nrtb_confreader_h

#include <string.h>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <common.h>
#include <logger.h>
#include <singleton.h>

namespace nrtb 
{

/** Reads command line and configuration file information.
 **
 ** For this NRTB implementation, this class is implemented as
 ** a singleton.
 ** 
 ** Values read are stored in a multimap of name/value pairs and
 ** may be accessed via provided iterators, or directly via the 
 ** [] operator, all(), get<T>() and getall<T>() methods. Automatic
 ** handling of include files and comments is provided as well. The 
 ** get<>() and getall<>() template methods provide easy and complete
 ** access to all types that have the ">>" stream operator
 ** defined, including all the C++ standard types.
 **/
class conf_reader
{
private:
  typedef std::multimap<std::string,std::string> value_list_type;
  value_list_type values;
public:
  /// conf_reader iterator, points to a conf_reader::pair
  typedef value_list_type::iterator iterator;
  /// conf_reader::pair is a std::pair<string,string>
  typedef std::pair<std::string,std::string> pair;
  /** No argument constructor; actually calls read() without a filename.
  **/
  conf_reader();
  /** NOP virtual destructor to allow safe inheritance.
  **/
  virtual ~conf_reader();
  /** Reads and parses the command line and the provided file.
  ** 
  ** Returns the number of values stored. argc and argv are, of 
  ** course the same names you used as arguments to main(). Therefore
  ** you can easily read all command line and configuration file 
  ** values like this:
  ** 
  ** int main(int argc, char* argv[])
  **
  ** {
  **
  ** 	conf_reader config;
  **
  ** 	config.read(argc,argv,"my_conf_file");
  **
  ** 	...
  **
  ** }
  ** 
  ** See read(filename,_clear) for a discription of how configuration
  ** files are structured and parsed. This method unconditionally
  ** clears the existing value list before starting. For command line
  ** arguments the following rules are true.
  ** 
  ** 1. The executable name (argv[0]) is stored as a value with the 
  **    name "__exec_name".
  **   
  ** 2. Command line arguments of the form "name=value" are parsed
  **    stored as named values.
  ** 
  ** 3. All other command line arguments are stored as names with 
  **    value = "".
  **    
  ** 4. A command line argument of form "configfile=filename" will
  **    override the filename supplied as an argument to this method.
  **    
  ** 5. In the case of duplicate command line arguments, the last one 
  **    specified wins.
  **    
  ** 6. In the case of names in the configuration file duplicating 
  **    names from the command line, the values from the command line
  **    dominate. If there were multiple values for a given name 
  **    specified in the file, only the first one (the one returned by
  **    the "[]" operator or get<>() method) is overridden.
  **/
  unsigned int read(int argc, char* argv[], 
	  const std::string & _filename);
  /// clears the name/value list.
  void clear();
  /// returns an iterator to the first item in the list.
  iterator begin();
  /// returns an iterator one past the end of the list.
  iterator end();
  /// True if there are no values, false otherwise.
  bool empty();
  /// Returns the number of values stored.
  unsigned int size();
  /** Returns the string value matching the supplied name.
  ** 
  ** NOTE: the use of get<T>(key) or get<T>(key,default) is preferred.
  ** This method is public to allow for specialized handling in the
  ** rare cases where it may be required.
  ** 
  ** If there are no matching names, "" is returned. Be aware that
  ** "" is a valid value, so you can not use this to verify that
  ** a given name was defined. Use exists() for that.
  **/
  std::string operator [] (const std::string & key);
  /** Returns all values associated with the supplied name.
  ** 
  ** NOTE: the use of getall<T>(key) is preferred. This method is 
  ** public to allow specialized handling in rare case where it
  ** may be required.
  ** 
  ** If there are no values defined  the strlist will be empty.
  **/
  strlist all(const std::string & key);
  /// True if the name exists, false otherwise.
  bool exists(const std::string & key);
  /** Use this to get all matching values.
  ** 
  ** Usage:
  ** 
  ** vector<type> mylist = conf_reader_object.getall<type>(key);
  ** 
  ** type can be any standard type (string, int, double, etc.) or any
  ** type for which the ">>" stream operator is defined. All values
  ** with matching names that can map to the requested type will be
  ** returned. Any that do not map will not be returned.
  **/
  template < class T >
    typename std::vector<T> getall(const std::string & key);
  /** Use this to get the matching value.
  ** 
  ** Useage:
  ** 
  ** type myvar = get<type>(key);
  ** 
  ** type can be any standard type (string, int, double, etc.) or any
  ** type for which the ">>" stream operator is defined. The return
  ** value is initialized to all zeros if no matching name is found or
  ** if the first value with that name does not map to the requested 
  ** type. For the numeric types that results in zero being returned,
  ** but be aware that more complex object may be in non-sensible
  ** states if they were not found or could not map. Use exists() to 
  ** verify the existance of a given name if needed.
  **/
  template < class T >
    T get(const std::string & key);
  /** Returns the value for the requested key, or the supplied default.
  ** 
  ** Works exactly like get<>() with the exception that if the key
  ** is not found in the list, the user supplied default value is
  ** returned instead of 0 or an empty string. If the value exists but
  ** can not map to the requested type, 0 or an empty string is 
  ** returned for the standard types.
  **/
  template <class T>
    T get(const std::string & key, const T & def);
private:
  std::string filename;
protected:
  /** Reads and parses a configuration file.
  ** 
  ** Returns the number of values stored.
  ** 
  ** The optional parameter _clear when true clears the currently
  ** stored list of values. if false the new values read will be added
  ** to the existing list.
  ** 
  ** If the file named can not be processed completely, a warning
  ** message is published to cerr. 
  ** 
  ** Configuration files are defined as follows.
  ** 
  ** 1. Files are read line by line. No value can cross a newline
  **    (and at least in this version) or include one. The good
  **    news is that you are allowed up to 16k for each line.
  **    
  ** 2. values are specifed in name/value pairs, one per line, with
  ** 	 the name first, followed by whitespace, followed by the 
  ** 	 value. Values may include whitespace. All leading and trailing
  ** 	 whitespace is removed from both the name and the value.
  ** 	 
  ** 3. Comments start with a \# symbol; all text following a \# is
  **    ignored. If you need to use \# in a name or value, escape
  **    it with a backslash instead.
  **    
  ** 4. Duplicate names are allowed.
  ** 
  ** 5. Names without values will be stored with "" as the value.
  ** 
  ** 6. A configuration file may include another configuration file
  **    automatically by the use of the "*INCLUDE" reserved name.
  **    When this name is found, it's value is used as the name of
  **    the new file to be read.
  **/
  unsigned int read(const std::string & _filename = "", bool _clear=true);
};

typedef singleton<conf_reader> global_conf_reader;

template < class T >
  typename std::vector<T> conf_reader::getall(const std::string & key)
{
  strlist tvals = all(key);
  std::vector<T> returnme;
  if (typeid(T) == typeid(std::string))
  {
    // T is a std::string.. we can do this quickly.
    strlist * wl = (strlist *) &returnme;
    *wl = tvals;
  }
  else
  {
    // T is non-string.. will require more playing around.
    unsigned int limit = tvals.size();
    for (unsigned int i = 0; i < limit; i++)
    {
      try
      {
	returnme.push_back(boost::lexical_cast<T>(tvals[i]));	
      }
      catch (...) {};
    };
  };
  return returnme;
};

template < class T >
  T conf_reader::get(const std::string & key)
{
  conf_reader & me = *this;
  std::string tval = me[key];
  T returnme;
  // initialize the return value to nulls
  // Needed for the numeric types, but bad for strings.
  if (typeid(T) != typeid(std::string))
  {
    // null out the working area (death for strings!)
    memset(&returnme,0,sizeof(T));
  };
  // This does appear to work for all the standard types.
  if (tval != "")
  {
    try
    {
      returnme = boost::lexical_cast<T>(tval);
    }
    catch (...) {};
  };
  return returnme;
};

template < class T >
  T conf_reader::get(const std::string & key, const T & def)
{
  if (exists(key)) { return get<T>(key); }
  else { return def; };
};

}; // namespace nrtb

#endif // nrtb_confreader_h

