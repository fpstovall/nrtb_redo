//
// C++ Interface: plugin_manager
//
// Description: 
//
//
// Author: Rick Stovall <rstovall@gabbie>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef plugin_manager_h
#define plugin_manager_h

#include "plugin_wrapper.h"
#include <map>

namespace NRTB
{
	/**
	  * Provides a simple mechanism for an application to
	  * load and manage plugins (shared objects in the unix lingo)
	  * which provide the application with functionality not
	  * not available at the time it was built.
	  *
	  * To be useable with this class a shared object must
	  * meet certain requirements which are outlined in the
	  * NRTB::plugin_wrapper class documentation. Example
	  * plugins are also provided.
	  */
	class plugin_manager
	{
		public:
			/// Typedef for return values
			typedef std::vector<std::string> strlist;
			/// parent for all exceptions thrown by this class
			class general_exception: public base_exception {};
			/// thrown if a specified plugin file was no found.
			class plugin_file_not_found: public general_exception {};
			/// thrown if a specified load path (directory) was not found.
			class path_not_found: public general_exception {};
			/// thown if a specified load path is not a directory.
			class not_a_directory: public general_exception {};
			/// thrown if a specifically requested plugin is not loaded.
			class plugin_not_found: public general_exception {};
			/**
			 * Simple constructor.
			 */
			plugin_manager();
			/**
			 * Constructs the class and then attempts to load all
			 * files found under the given path (recursively) as
			 * plugins.Non-plugin files will be ignored, but
			 * problems reading the given path (path not found,
			 * insufficient permissions, etc.) will cause an
			 * exception to be thrown.
			 *
			 * @param loadpath The top of the file tree to be searched.
			 */
			plugin_manager(const boost::filesystem::path loadpath);
			/**
			 * Unloads all managed plugins and destructs the
			 * plugin_manager. Probably not a good thing to do
			 * while any of the plugins are in use.
			 */
			virtual ~plugin_manager();
			/**
			 * Attempts to load all files found under the
			 * given path (recursively) as plugins.Non-plugin
			 * files will be ignored, but problems reading the
			 * given path (path not found, insufficient
			 * permissions, etc.) will cause an exception to be thrown.
			 *
 			 * If two plugins have the same class and name, the one
			 * with the highest version number will be kept and
			 * the other(s) discarded.
			 *
			 * @param loadpath The top of the file tree to be searched.
			 */
			void load_all(const boost::filesystem::path loadpath);
			/**
			 * Attempts to load a plugin from the fully qualified
			 * file path given. Exceptions will be thrown if there
			 * are any problems reading the file, or if it is not a
			 * valid plugin (by plugin_wrapper standards).
			 *
			 * If two plugins have the same class and name, the one
			 * with the highest version number will be kept and
			 * the other(s) discarded.
			 * 
			 * @param loadpath A fully qualified file path.
			 */
			void load_one(const boost::filesystem::path loadpath);
			/**
			 * Releases all plugins currently being managed. Probably
			 * not a good thing to do if any of them are being used.
			 */
			void release_all();
			/**
			 * Attempts to release the single plugin matching the
			 * provided plugin class and name. An exception will be thrown
			 * if the plugin is not found.
			 *
			 * @param pclass The class of the plugin to release.
			 * @param name The name of the plugin to release.
			 */
			void release_one(const std::string pclass, const std::string name);
			/**
			 * Dumps a list of all loaded plugins, intended for
			 * diagnostic purposes. The list is formatted:
			 *
			 *     plugin_class::plugin_name<eol>
			 *
			 * with one line for each plugin loaded.
			 *
			 * @return A string contianing a formatted plugin listing.
			 */
			std::string dump();
			/**
			 * Returns a strlist (a vector<string>) containing
			 * all unique plugin class names loaded, in alpha order.
			 *
			 * @return A list of all unique plugin classes loaded.
			 */
			strlist get_class_names();
			/**
			 * Returns a strlist (a vector<string>) containing
			 * the names of all plugins loaded of the given plugin class.
			 *
			 * @param pclass The name of the plugin class to be searched.
			 * @return A list of all plugins loaded for the given plugin class.
			 */
			strlist get_class_plugin_names(const std::string pclass);
			/**
			 * Returns true if a plugin with the given class
			 * and name exists, or faise if not.
			 *
			 * @param pclass The class name of the plugin to be checked.
			 * @param pname The name of the plugin to be checked
			 * @return True if the plugin is laoded, false otherwise.
			 */
			bool exists(const std::string pclass, const std::string pname);
			/**
			 * Returns a boost::shared_ptr<T> to a new instance
			 * of the class provided by the plugin specified by
			 * the plugin_class and plugin_name provided.
			 *
			 * Usage example:
			 *    myTypePointer = create_plugin_instance<mytype>(class,name);
			 *
			 * This routine will succeed unconditionally or throw
			 * one of several possible exceptions. In no case will a
			 * null pointer or improperly constructed instance be
			 * returned.
			 *
			 * @param plugin_class The class name of the plugin desired
			 * @param plugin_name The name of the plugin desired.
			 * @return A boost::shared_ptr<T> to a ready to use instance.
			 */
			template <class T> boost::shared_ptr<T> create_plugin_instance
			(
				const std::string plugin_class,
				const std::string plugin_name
			)
			{
				typedef boost::shared_ptr<T> returntype;
				if (!exists(plugin_class, plugin_name))
				{
					plugin_not_found e;
					e.store(plugin_class+"::"+plugin_name);
					throw e;
				};
				returntype returnme =
					plugins[plugin_class][plugin_name]->create_instance<T>();
				return returnme;
			};
		protected:
			// smart pointer to a plugin_wrapper
			typedef boost::shared_ptr<plugin_wrapper> wrapper_p;
			// type used contain plugins of the same type
			typedef std::map<std::string, wrapper_p> plugins_by_name;
			// type used to contain lists of types of plugins.
			typedef std::map<std::string, plugins_by_name> plugins_list_by_class;
			// iterator providing access to "plugins_by_name" lists by class
			typedef plugins_list_by_class::iterator class_iterator;
			// iterator providing access to individual plugins by name
			typedef plugins_by_name::iterator plugin_iterator;
			// container for all plugins being managed.
			plugins_list_by_class plugins;
	};
};

#endif //plugin_manager_h
