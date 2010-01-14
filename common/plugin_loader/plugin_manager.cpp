//
// C++ Implementation: plugin_manager
//
// Description: 
//
//
// Author: Rick Stovall <rstovall@gabbie>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "plugin_manager.h"

//use boost::filesystem;
//use boost::filesystem::directory_iterator;


// TODO: review to make sure all methods from the .h are implemented.
namespace NRTB
{

	plugin_manager::plugin_manager()
	{
		// nop in this version.
	};

	plugin_manager::plugin_manager(const boost::filesystem::path loadpath)
	{
		plugin_manager();
		load_all(loadpath);
	};
	
	plugin_manager::~plugin_manager()
	{
		release_all();
	};
	
	void plugin_manager::load_all(const boost::filesystem::path loadpath)
	{
		// check to if loadpath really exists
		if (!boost::filesystem::exists(loadpath))
		{
			path_not_found e;
			e.store(loadpath.native_directory_string());
			throw e;
		};
		if (!is_directory(loadpath))
		{
			not_a_directory e;
			e.store(loadpath.native_file_string());
			throw e;
		};
		// set up for the scan
		boost::filesystem::directory_iterator end; // points to the end of the list
		boost::filesystem::directory_iterator current(loadpath);
		while (current != end)
		{
			if (is_directory(*current))
			{
				// recursive call to process the directory.
				load_all(*current);
			}
			else
			{
				// try to load the plugin
				try
				{
					load_one(*current);
				}
				catch (plugin_wrapper::invalid_plugin & e)
				{
					// take no action in this caee...
				};
			};
			current++;
		};
	};
		
	void plugin_manager::load_one(const boost::filesystem::path loadpath)
	{
		// check to if loadpath really exists
		if (!boost::filesystem::exists(loadpath))
		{
			path_not_found e;
			e.store(loadpath.native_directory_string());
			throw e;
		};
		if (boost::filesystem::is_directory(loadpath))
		{
			plugin_file_not_found e;
			e.store(loadpath.native_file_string());
			throw e;
		};
		/* NOTE: since wrapper_p is a smart pointer, any memory
			allocated to it will be discarded if an exception
			is thrown in the following section.
		*/
		// Create the plugin_wrapper.
		wrapper_p new_plugin(new plugin_wrapper);
		// Load it... will throw if somehing's not right.
		new_plugin->load(loadpath);
		// Store it in our list.
		plugins[new_plugin->get_type()][new_plugin->get_name()] = new_plugin;
	};

	void plugin_manager::release_all()
	{
		// this will call the destructor and deallocate all
		// plugin_wrappers currently loaded.
		plugins.clear();
	};
	
	void plugin_manager::release_one(const std::string pclass,
		const std::string name)
	{
		// TODO: more here.
		plugins[pclass].erase(name);
	};

	std::string plugin_manager::dump()
	{
		// TODO: More to do here.
	};

}; // namespace NRTB

