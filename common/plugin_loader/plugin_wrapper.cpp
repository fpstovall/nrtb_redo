//
// C++ Implementation: plugin_wrapper
//
// Description: 
//
//
// Author: Rick Stovall <rstovall@gabbie>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "plugin_wrapper.h"

namespace NRTB
{

	plugin_wrapper::plugin_wrapper()
	{
		// Load the "constants"
		std::string name_key = "PLUGIN_NAME";
		std::string type_key = "PLUGIN_TYPE";
		std::string version_key = "PLUGIN_VERSION";
		std::string factory_key = "PLUGIN_FACTORY";
		// clear all the associated fields.
		plugin_type = "";
		plugin_name = "";
		plugin_version = 0.0;
		plugin_path = "";
		// close the handle if it's already assigned.
		if (dl_handle)
		{
			dlclose(dl_handle);
		};
		dl_handle = 0;
	};

	plugin_wrapper::plugin_wrapper(boost::filesystem::path loadpath)
	{
		load(loadpath);
	};

	plugin_wrapper::~plugin_wrapper()
	{
		// close the handle if it's already assigned.
		if (dl_handle)
		{
			dlclose(dl_handle);
		};
		dl_handle = 0;		
	};

	void plugin_wrapper::load(boost::filesystem::path loadpath)
	{
		// establish initial conditions.
		plugin_wrapper();
		if (loadpath.empty())
		{
			// an empty path could expose security holes.
			file_not_found e;
			e.store("Illegal null path provided");
			throw e;
		};
		dlerror();
		try
		{
			// call dlopen to open the lib
			dl_handle = dlopen(loadpath.string().c_str(), RTLD_LAZY);
			if (!dl_handle)
			{
				throw new file_not_found;
			};
			// load the fields
			plugin_type = get_value<std::string>(type_key);
			plugin_name = get_value<std::string>(name_key);
			plugin_version = get_value<float>(version_key);
			plugin_path = loadpath;
		}
		catch (file_not_found &e)
		{
			// is it a missing file or a bad file?
			if (!boost::filesystem::exists(loadpath))
			{
				e.store(loadpath.string());
				throw e;
			}
			else
			{
				invalid_plugin ie;
				ie.store(dlerror());
				throw ie;
			};
		}
		catch (invalid_plugin & e)
		{
			if (e.comment() == "")
			{
				e.store(dlerror());
			};
			throw e;
		}
		catch (std::exception &e)
		{
			general_exception ge;
			ge.store(e.what());
			throw ge;
		};
	};

	std::string plugin_wrapper::get_type()
	{
		if (!dl_handle)
		{
			throw new not_loaded;
		};
		return plugin_type;
	};

	std::string plugin_wrapper::get_name()
	{
		if (!dl_handle)
		{
			throw new not_loaded;
		};
		return plugin_name;
	};

	float plugin_wrapper::get_version()
	{
		if (!dl_handle)
		{
			throw new not_loaded;
		};
		return plugin_version;
	};

	boost::filesystem::path plugin_wrapper::get_path()
	{
		if (!dl_handle)
		{
			throw new not_loaded;
		};
		return plugin_path;
	};
	
} // namespace NRTB
