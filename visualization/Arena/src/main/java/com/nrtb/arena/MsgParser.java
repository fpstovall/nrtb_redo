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


import java.io.*;
import java.lang.*;
import java.util.*;

public class MsgParser {

	private DebugLvl parser_dbug;
	private String[] parsedStr = null;
	private static MsgParser ref;

	private MsgParser ()  {
		parser_dbug = DebugLvl.getDebugLvlObject();    // singleton
	}

	public static synchronized 
					MsgParser getParserObject()
    {
    	if (ref == null)
        	// it's ok, we can call this constructor
        	ref = new MsgParser();
    	return ref;
    }

	public Object clone()
			throws CloneNotSupportedException
  	{
    	throw new CloneNotSupportedException(); // Don't allow cloning of a singleton
  	}
  	
	public String[] parseMsg (String inStr)  { 
  		if (inStr != null)  {
  			parsedStr = inStr.split("[,]+");
  		}  else  {
  			parsedStr = null;
  		}

		return (parsedStr); 
	}
}
