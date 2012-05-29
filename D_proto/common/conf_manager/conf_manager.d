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
  
}