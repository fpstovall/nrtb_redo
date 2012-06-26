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

import std.stdio;
import std.conv;
import std.array;
import std.getopt;

/** Reads command line and configuration file information.
*
* For this NRTB implementation, this class is implemented as
* a singleton.
*
* Values read are stored in a multimap of name/value pairs and
* may be accessed via provided iterators, or directly via the
* [] operator, all(), get<T>() and getall<T>() methods. Automatic
* handling of include files and comments is provided as well. The
* get<>() and getall<>() template methods provide easy and complete
* access to all types that have the ">>" stream operator
* defined, including all the C++ standard types.
**/

class conf_reader
{
  alias string[string] mylist;
  
  this(string[] args)
  {
    string c_file = "nrtb.config";
    string c_path = ".";
    // store off the program name before we start
    values["__app_name"] = args[0];
    args = args[1 .. $];
    // get the traditional args.
    getopt(args,
      std.getopt.config.passThrough,
      "config_file", &c_file,
      "config_path", &c_path
    );
    // process the remaining elements
    foreach(arg; args)
    {
      parse_arg(arg);
    };
  }

  private void parse_arg(in string arg)
  {
    string[] tokens = split(arg,"=");
    if (tokens.length == 1)
    {
      values[tokens[0]] = "1";
    }
    else if (tokens.length == 2)
    {
      auto key = tokens[0];
      auto val = tokens[1];
      // check for reserved words
      if (key == "include")
        load_file(val);
      else
        values[key] = val;
    }
  }

  private void load_file(in string file_name)
  {
    // need to add the logic to read files here.
  }

  mylist get_list() { return values.dup; };

  T get(T,E)(in string key,in E _default)
    if (typeof(T = E) == typeof(T))
  {
    if ((key in values) != null)
      return to!T(values[key]);
    else
      return _default;
  }

  private mylist values;

}

