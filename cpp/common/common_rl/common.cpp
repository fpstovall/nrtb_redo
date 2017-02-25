/***********************************************
 T his file is part of the NRTB project (https://github.com/fpstovall/nrtb_beta).
 
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

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "common.h"

using namespace std;

namespace nrtb
{

const string __ricks_handy_upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string __ricks_handy_lower = "abcdefghijklmnopqrstuvwxyz";

string changecase(const string &s,const string &upper, 
		  const string &lower)
{
  try
  {
    string returnme = "";
    for (int i = 0; i < s.size(); i++)
    {
      unsigned long int loc = lower.find(s[i]);
      if (loc == string::npos)
      {
	returnme += s[i];
      }
      else
      {
	returnme += upper[loc];
      };
    };
    return returnme;
  }
  catch (...)
  {
    throw translate_exception();
  };
};

string upcase(const string &s)
{
  return changecase(s,__ricks_handy_upper,__ricks_handy_lower);
};

string downcase(const string &s)
{
  return changecase(s,__ricks_handy_lower,__ricks_handy_upper);
};


const string __ricks_handy_hexits = "0123456789ABCDEF";

string gsub(const string & target, const string & findme,
	    const string & putme)
{
  // error checking
  if (findme.empty() || target.empty())
  {
    throw gsub_exception();
  };
  string returnme = "";
  unsigned long int okay_through = 0;
  unsigned long int step = findme.length();
  unsigned long int where = target.find(findme,okay_through);
  while (where != string::npos)
  {
    returnme += target.substr(okay_through,where-okay_through) + putme;
    okay_through = where + step;
    where = target.find(findme,okay_through);
  };
  returnme += target.substr(okay_through);
  return returnme;
};

strlist split(const string & source, const char token)
{
  strlist returnme;
  returnme.clear();
  if (source.size() > 0)
  {
    unsigned long int loc;
    unsigned long int processed = 0;
    try
    {
      loc = source.find(token,processed);
      while (loc != string::npos)
      {
	returnme.push_back(source.substr(processed,loc-processed));
	processed = loc + 1;
	loc = source.find(token,processed);
      }
      returnme.push_back(source.substr(processed));
    }
    catch (...)
    {
      throw split_exception();
    };
  };
  return returnme;
}; 

string trim(std::string s)
{
  const string ws = "\t\n ";
  unsigned long int where = s.find_first_not_of(ws);
  if (where != string::npos)
  {
    s.erase(0,where);
  };
  where = s.find_last_not_of(ws);
  if (where != string::npos)
  {
    s.erase(where+1);
  };
  return s;
};

string mconvert(const string &s)
{
  /* This function is designed to escape strings destined for 
    * use in SQL queries and the like.  Specifically, the following 
    * is done to the string passed in:
    *   1. "\" is replaced with "\\"
    *   2. "'" is replaced with "\'"
    *   3. """ (double quote) is replaced with "\""
    *   4. 0x00 is replaced with "\"+0x00.
  */
  string returnme = "";
  if (s.length() > 0)
  {
    returnme = gsub(s,"\\","\\\\");
    returnme = gsub(returnme,"'", "\\'");
    returnme = gsub(returnme,"\"","\\\"");
    // special handling is required to make strings including
    // 0x00; this is the kludge I found to work at 2200 tonight.
    string findme = " "; findme[0] = 0;
    returnme = gsub(returnme,findme,"\\0");
  };
  return returnme;
};

string dateflip(string date, const string & sep) 
{
  string out;
  try
  {
    strlist temp = split(date,'-');
    out = temp[1]+ sep + temp[2] + sep + temp[0];
  }
  catch (...)
  {
    throw dateflip_exception();
  };
  return out;
}; // string dateflip
   
string http_hextochar(string s)
{
  try
  {
    s = upcase(s);
    unsigned char v = (16 * __ricks_handy_hexits.find(s[0])) 
	+ __ricks_handy_hexits.find(s[1]);
    return s = v;
  }
  catch (...)
  {
    throw hextrans_exception();
  };
}; // string hextochar

string http_chartohex(const string &s)
{
  string out;
  try
  {
    for (long int i=0; i < s.length() ; i++ ) {
      unsigned char v = s[i];
      div_t hexval = div(v,16);
      out += __ricks_handy_hexits[hexval.quot];
      out += __ricks_handy_hexits[hexval.rem];
    }; /* endfor */
  }
  catch (...)
  {
    throw hextrans_exception();
  };
  return out;
};// string chartohex()

string http_enhex(const string & s)
{
  string out;
  try
  {
    for (long int i=0; i < s.length() ; i++ ) 
    {
      unsigned char v = s[i];
      div_t hexval = div(v,16);
      out += "%";
      out += __ricks_handy_hexits[hexval.quot];
      out += __ricks_handy_hexits[hexval.rem];
    }; /* endfor */
  }
  catch (...)
  {
	  throw hextrans_exception();
  };
  return out;
}; // string enhex()

string http_unhex(string s)
{
  try
  {
    while (s.find('%') != string::npos) 
    {
      int where = s.find('%');
      string hexchar = s.substr(where+1,2);
      s.erase(where,3);
      s.insert(where,http_hextochar(hexchar));
    }; /* endwhile */
  }
  catch (...)
  {
	  throw hextrans_exception();
  };
  return s;
};// string unhex()


} // namespace nrtb
