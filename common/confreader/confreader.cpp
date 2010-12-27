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

// see confreader.h for documentation

#include "confreader.h"
#include <fstream>
#include <iostream>
#include <common.h>
#include "Poco/Logger.h"

using namespace std;
using namespace nrtb;

const std::string logname = "conf_reader";


namespace nrtb
{

conf_reader::conf_reader()
{
	Poco::Logger& logger = Poco::Logger::get(logname);
	logger.information("conf_reader instanciated.");
};

conf_reader::~conf_reader() {};

unsigned int conf_reader::read(const std::string & _filename, bool _clear)
{
	Poco::Logger& logger = Poco::Logger::get(logname);
	logger.information("Reading from \"" + _filename + "\".");
	if (_filename != "") { filename = _filename; };
	if (filename != "")
	{
		try
		{
			ifstream cfile(filename.c_str());
			if (!cfile) throw base_exception();
			if (_clear) values.clear();
			// read the file line by line, ignoring comments.
			char inbuff[16*1024];
			while (cfile)
			{
				cfile.getline(inbuff, 16*1024);
				string in = inbuff;
				// truncate at the start of a comment.
				unsigned long int where = in.find("#");
				while (where != string::npos)
				{
					if (in[where-1] == '\\')
					{
						// comment char was escaped.. ignore.
						where = in.find("#",where+1);
					}
					else
					{
						// truncate the string at this point.
						in.erase(where);
						where = string::npos;
					};
				};
				// see if we can parse what's left.
				in = trim(in);
				unsigned long int split_point = in.find_first_of("\t ");
				if (split_point != string::npos)
				{
					// okay.. get the fields.
					pair arg;
					arg.first = gsub(trim(in.substr(0,split_point)),"\\#","#");
					arg.second = gsub(trim(in.substr(split_point)),"\\#","#");
					// is this an include directive?
					if (arg.first == "*INCLUDE")
					{
						read(arg.second,false);
					} 
					else if (arg.first != "")
					{
						values.insert(arg);
					};
				};
			};
		}
		catch (...)
		{
//			Poco::Logger& logger = Poco::Logger::get(logname);
			logger.warning("Problems reading configuration file \""
				+ filename + "\"; data may be incomplete.");
		}
	};
	return values.size();
};

unsigned int conf_reader::read(int argc, char * argv[], 
	const string & _filename)
{
	Poco::Logger& logger = Poco::Logger::get(logname);
	logger.information("Reading from command line.");
	clear();
	filename = _filename;
	value_list_type cvars;
	// read values from the command line first.
	for (int i = 0; i < argc; i++)
	{
		string instring = argv[i];
		if (i == 0)
		{
			instring = "__exec_name="+instring;
		};
		strlist t = split(instring,'=');
		if (t.size() > 0)
		{
			// build the new insert pair;
			pair newval;
			newval.first = t[0];
			// assemble the value
			// (allows for including "=" in the argument)
			for (unsigned int l = 1; l < t.size(); l++)
			{
				newval.second += t[l];
				if (l < (t.size() -1) )
				{
					newval.second += "=";
				};
			};
			// store this in the list
			trim(newval.first);
			trim(newval.second);
			cvars.insert(newval);
			// is this a config file name?
			if (newval.first == "configfile")
			{
				filename = newval.second;
			};
		};
	}; // read the command line arguments.
	// read the file args if any.
	read(filename,false);
	// override the first instance of any value found in configs
	// or insert as appropriate.
	iterator c = cvars.begin();
	iterator e = cvars.end();
	while (c != e)
	{
		iterator here = values.find(c->first);
		if (here != values.end())
		{
			here->second = c->second;
		}
		else
		{
			values.insert(*c);
		};
		c++;
	};
	std::stringstream message;
	message << "Read " << values.size() << " parameters.";
	logger.information(message.str());
	return values.size();
};

void conf_reader::clear()
{
	values.clear();
};

conf_reader::iterator conf_reader::begin()
{
	return values.begin();
};

conf_reader::iterator conf_reader::end()
{
	return values.end();
};

bool conf_reader::empty()
{
	return values.empty();
};

unsigned int conf_reader::size()
{
	return values.size();
};

strlist conf_reader::all(const std::string & key)
{
	strlist returnme;
	iterator current = values.find(key);
	iterator e = values.end();
	while ((current != e) && (current->first == key))
	{
		returnme.push_back(current->second);
		current++;
	};
	return returnme;
};

string conf_reader::operator [] (const std::string & key)
{
	iterator res = values.find(key);
	string returnme = "";
	if (res != values.end()) returnme = res->second;
	return returnme;
};

bool conf_reader::exists(const std::string & key)
{
	return (values.find(key) != values.end());
};

} // namespace nrtb
