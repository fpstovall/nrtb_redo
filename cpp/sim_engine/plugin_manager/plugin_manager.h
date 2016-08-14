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
 
#ifndef plugin_manager_header
#define plugin_manager_header

#include <boost/dll.h>
#include <base_object.h>

namespace nrtb
{

class plugin_manager
{
private:
	struct plugin_rec
	{
		std::string name;
		effector_factory factory;
	};
	std::map<std::string, plugin_rec> eff_plugins;
	void add_plugin(std::string name);
	void remove_plugin(std::string name);
public: 
	// catch this to handle all plugin_manager errors.
	class general_error :: public base_exception {};
	// catch this to handle naming issues.
	class not_found :: public general_error {};
	// catch this to handle dll load issues.
	class load_error :: public general_error {};
	plugin_manager();
	~plugin_manager();
	effector_p get_effector(std::string name);
};

} // namepace nrtb

#endif // plugin_manager_header
