//
// C++ Interface: plugin_wrappr
//
// Description: 
//
//
// Author: Rick Stovall <rstovall@gabbie>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef plugin_wrapper_h
#define plugin_wrapper_h

#include <dlfcn.h>
#include <string>
#include <ricks_handy.h>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/convenience.hpp>

namespace NRTB
{
 	/**
 	 * A container for a single plugin. A plugin is defined as a
 	 * shared object file which provides a class, an associated factory
 	 * function, and several housekeeping constants, as follows:
	 *	
     *	1. A std::string identifying the class factory named "PLUGIN_FACTORY"
     *
     *	2. A plugin name std::string named "PLUGIN_NAME"
     *
     *	3. A plugin type std::stirng named "PLUGIN_TYPE"
     *
     *	4. A plugin version number float named "PLUGIN_VERSION"
     *	
	 * This class is not designed to instanticated directly by user code.
	 * Instead, it will be intanciated by the plugin_manager, which acts as
	 * a container for many plugins and provides access to them.
	 *
	 * In call cases except load(), when an exception is thown from a method,
	 * the instance is left the state it was in at the start of the method.
 	**/
 	class plugin_wrapper
	{
		public:
			/// Parent for all plugin_wrapper exceptions.
			class general_exception: public base_exception {};
			/// Thrown if a file is not found at the requested path.
			class file_not_found: public general_exception {};
			/// Thrown if a requested file does not contain a valid plugin.
			class invalid_plugin: public general_exception {};
			/// Thrown if queries are called when no data is present.
			class not_loaded: public general_exception {};
			/**
			 * Default constructor; NOP.
			 */
			plugin_wrapper();
			/**
			 * This constructor attempts to load a plugin at
			 * the argument provided. Actually calls load() to
			 * to do the work.
			 * 
			 * @param loadpath  Should be point to a valid plugin file
			 */
			plugin_wrapper(boost::filesystem::path loadpath);
			/**
			 * Unloads any plugin, if contained.
			 */
			virtual ~plugin_wrapper();
			/**
			 * Discards the current contents of this wrapper, if any,
			 * and then attempts to load the plugin file provided. If there
			 * are any problems, an appropriate exception will be thrown.
			 *
			 * @param loadpath  Should point to a valid plugin file.
			 */
			virtual void load(boost::filesystem::path loadpath);
			/**
			 * Returns the pligin type associated with the loaded plugin,
			 * or throws if one is not loaded. The plugin type is a string
			 * indicating the what services the plugin provides and in
			 * most cases will have no meaning outside of the application
			 * context.
			 *
			 * For example, a plugin providing printing functionality
			 * may have a type of "printer", while one that provides
			 * database access may have a type of "db_interface".
			 *
			 * @return A std::string indicating plugin type.
			 */
			virtual std::string get_type();
			/**
			 * Returns the name of the particular plugin loaded, or
			 * throws if one is not loaded. The name is an arbitrary string
			 * which uniquely identifies a particular plugin within a type.
			 *
			 * For example, a plugin providing printing functionality to 
			 * a PDF file may have a type of "printer" and a name of "PDF",
			 * while another "printer" plugin providing output to doc
			 * files may have the name "WordDoc".
			 *
			 * @return A std::string indicating the plugin name.
			 */
			virtual std::string get_name();
			/**
			 * Returns the version of the particular plugin loaded, or
			 * throws if one is not loaded. The version is an arbitrary
			 * float value which uniquely identifies a particular version
			 * of a plugin within a name and type.
			 *
			 * @return A float indicating the plugin version.
			 */
			virtual float get_version();
			/**
			 * Returns the fully qualified path of the plugin loaded, or
			 * throws if one is not loaded.
			 *
			 * @return A boost::filesystem::path to the plugin file. 
			 */
			boost::filesystem::path get_path();
			/**
			 * Calls the class factory and returns shared_ptr to an
			 * instance of the type requested. For reliable operation,
			 * the type provided the template must be compatable with
			 * that actually implemented in the plugin.
			 *
			 * An appropriate exception will be thrown if there are issues.
			 *
			 * Usage: mytype myvar createInstance<mytype>();
			 *
			 * @return A boost::shared_ptr to an instanace of
			 * the type provided.
			 */
			template <class T> boost::shared_ptr<T> create_instance()
			{
				if (!dl_handle)
				{
					not_loaded e;
					e.store("create_instance");
					throw e;
				};
				typedef T (*factory_type)();
				typedef boost::shared_ptr<T> my_pointer;
				my_pointer returnme;
				try
				{
					factory_type factory = get_value<factory_type>(factory_key);
					returnme = factory();
				}
				catch (std::exception & e)
				{
					// Let the caller sort it out.
					throw;
				}
				catch (...)
				{
					// most likely a bad problem in the factory function.
					invalid_plugin e;
					e.store("create_instance caught an unknown exception");
					throw e;
				};
				return returnme;
			};

		protected:
			// gets indivdual field values from the shared lib.
			template <class T> T get_value(const std::string & key)
			{
				typedef T * my_pointer;
				T returnme;
				dlerror();
				my_pointer value = (my_pointer) dlsym(dl_handle,key.c_str());
				std::string check_error = dlerror();
				if (!check_error.empty())
				{
					invalid_plugin e;
					e.store(check_error+" ("+key+")");
					throw e;
				};
				if (!value)
				{
					invalid_plugin e;
					e.store("("+key+") returned a null value");
					throw e;
				};
				returnme = *value;
				return returnme;
			};
		
		private:
			// The following are used to cache plugin data after load.
			void * dl_handle;
			std::string plugin_type;
			std::string plugin_name;
			float plugin_version;
			boost::filesystem::path plugin_path;
			// The following define what labels are expected to be defined.
			static const std::string name_key;
			static const std::string type_key;
			static const std::string version_key;
			static const std::string factory_key;
	};

} // nameplace NRTB

#endif // plugin_wrapper_h
