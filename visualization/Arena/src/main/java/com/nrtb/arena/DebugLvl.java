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


import java.io.*;
import java.lang.*;

public class DebugLvl {

	private int debugVal = 0;
    private static DebugLvl ref;
	
	private DebugLvl (int lvl)  {
		debugVal = lvl;
	}

	public static synchronized 
					DebugLvl getDebugLvlObject()
    {
    	if (ref == null)
        	// it's ok, we can call this constructor
        	ref = new DebugLvl(0);
    	return ref;
    }

	public static synchronized 
					DebugLvl getDebugLvlObject(int lvl)
    {
    	if (ref == null)
        	// it's ok, we can call this constructor
        	ref = new DebugLvl(lvl);
    	return ref;
    }

	public Object clone()
			throws CloneNotSupportedException
  	{
    	throw new CloneNotSupportedException(); // Don't allow cloning of a singleton
  	}
  
	public void print (int ii, int bId, String str)  {
		if (ii <= debugVal)  {
			System.out.println (" " + bId + ": " + str);	
		}
	}
	
	public int levelIs () { return (debugVal); }
}
	 