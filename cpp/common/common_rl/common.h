/***********************************************
 T his file is part of the NRTB project (https://*launchpad.net/nrtb).
 
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

// prevent multiple definitions
#ifndef __ga_common_h
#define __ga_common_h 1

#include <string>
#include <map>
#include <vector>
#include <chrono>

namespace nrtb
{

// throwable exception classes

/** Provides a base exception class for use in HCS developed programs.
 ** 
 ** Usage: Inherit your exception classes from this class. When throwing a 
 ** derived exception you'll have the option of either instanciating the 
 ** exception with a comment or not; in any case the time the exception was
 ** instanciated will be recorded. Later, when catching the exception you can 
 ** get the time the exception was created and any text the thrower may have
 ** provided.
 **/
class base_exception: public std::exception
{
protected:
  typedef std::chrono::high_resolution_clock myclock;
  typedef std::chrono::high_resolution_clock::time_point mark;
  typedef std::chrono::microseconds ms;
  mark created {myclock::now()};
  std::string _text;
  
public:
  /// NOP virtual distructor for safe inheritance
  virtual ~base_exception() throw() {};
  /** Stores a comment string.
   **/
  void store(const std::string & s) { _text = s; };
  /** Returns the value stored at exception creation.
   **/
  std::string comment() { return _text; };
  /** Returns the time the exception was created.
   **/
  mark creation_time() { return created; };
  /** Returns the age of the exception 
   **/
  ms age_in_ms() 
  { 
    return std::chrono::duration_cast<ms>(myclock::now() - created); 
  };
};

// Thrown by gsub() in cases of unexpectaed error.
class gsub_exception: public nrtb::base_exception {};
// Thrown by split() in cases of unexpected error.
class split_exception: public nrtb::base_exception {};
// Thrown by dateflip in cases of unexpected error.
class dateflip_exception: public nrtb::base_exception {};
// Thrown by upcase() and downcase() in cases of unexpected error.
class translate_exception: public nrtb::base_exception {};
// Thrown by http_* functions in cases of unexpected error.
class hextrans_exception: public nrtb::base_exception {};
// Thrown by the biased_rand* functions in cases of unexpected error.
class rand_exception: public nrtb::base_exception {};

// handy commonly used maps
/// A map of ints keyed by ints.
typedef std::map<int,int> num_map;
/// A map of ints keyed by strings.
typedef std::map<std::string,int> string_map;

/// the return type for split.
typedef std::vector<std::string> strlist;

/** Converts the alpha components of s to all upper case.
 ** 
 ** May throw a nrtb::translate_exception if there is a problem.
 **/
std::string upcase(const std::string &s);

/** Converts the alpha components of s to all lower case.
 ** 
 ** May throw a nrtb::translate_exception if there is a problem.
 **/
std::string downcase(const std::string &s);

/** Replaces all instances of findme in target with putme.
 ** 
 ** May throw a nrtb::gsub_exception if there is a problem.
 **/
std::string gsub(const std::string & target, const std::string & findme, 
	const std::string & putme);

/** Splits the string source using token as a delimiter and returns
 ** the results in vector of strings.
 ** 
 ** May throw a nrtb::split_exception if there is a problem.
 **/
strlist split(const std::string & source, const char token);

/// Performs SQL escaping on the string.
std::string mconvert(const std::string & s);

/// Removes white space from both ends of a string.
std::string trim(std::string s);

/** Takes an ISO standard date (YYYY-MM-DD) and returns MM-DD-YYYY.
 ** 
 ** May throw a nrtb::dateflip_exception if there is a problem.
 **/
std::string dateflip(std::string date, const std::string & sep="-"); 

/** Takes a string representing one HTTP hex-encoded byte and returns
 ** the appropriate character. This will only operate on the first two
 ** bytes of s, and will throw a nrtb_hextrans_exception if less 
 ** than two bytes are supplied.
 ** 
 ** May throw a nrtb::hextrans_exception if there is a problem.
 **/
std::string http_hextochar(std::string s);

/** Takes a string and returns it hex-encoded (i.e. " " returns "20").
 ** 
 ** This function is somewhat misnamed due to hysterical reasons. In 
 ** truth it simply returns the hex equivilent of the input. Use 
 ** http_enhex() when you actually need valid HTTP hex encoding. 
 ** On the other hand, this function is quite
 ** useful if you need to hex up a series of bytes and don't need the 
 ** overhead of the extra byte per character of true HTTP hex encoding.
 ** 
 ** May throw a nrtb::hextrans_exception if there is a problem.
*/
std::string http_chartohex(const std::string &s);

/** Takes a string and returns it in HTTP hex-encoded format (" " returns "%20").
 ** 
 ** May throw a nrtb::hextrans_exception if there is a problem.
 **/
std::string http_enhex(const std::string &s);

/** Takes a string and returns it with all HTTP hex-encoded bytes changed to 
 ** their normal single byte representation.
 ** 
 ** May throw a nrtb::hextrans_exception if there is a problem.
 **/
std::string http_unhex(std::string s);

} // namespace nrtb
#endif /* __ga_common_h */ 
