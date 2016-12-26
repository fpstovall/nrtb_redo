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

// see plugin_manager.h for documentation

#include "plugin_manager.h"
#include <sstream>

using namespace nrtb;

plugin_manager::plugin_manager();

plugin_manager::~plugin_manager() {};

effector_p plugin_manager::get_effector(std::string name);

void plugin_manager::add_plugin(std::string name);

void plugin_manager::remove_plugin(std::string name);


using namespace nrtb;


