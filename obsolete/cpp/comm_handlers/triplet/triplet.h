/***********************************************
 This file is part of the NRTB project (https://launchpad.net/nrtb).
 
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
 
#ifndef triplet_header
#define triplet_header

#include <triad.h>
#include <physics_common.pb.h>

namespace nrtb_com {

// -- typedefs to be used by callers
typedef nrtb::triad<long double> triplet;

class com_triplet
{
public:
  triplet internal;

  com_triplet();
  com_triplet(nrtb_msg::triplet* ext);
  virtual ~com_triplet() {};
  
  void set_from_message(nrtb_msg::triplet* ext);
  void load_message(nrtb_msg::triplet* ext);
  
  bool operator ==(com_triplet & a);
};

} // namespace nrtb::com

#endif // triplet_header
