/***********************************************
This file is part of the NRTB project (https://github.com/fpstovall/nrtb_beta).

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

public class ScreenView {

	private static ScreenView ref;

	private double screenCenter[] = new double[2];
	
	private double screenSizeX, screenSizeY;
	private double adjustedScreenSizeX, adjustedScreenSizeY;
	private DebugLvl svDvl;
	
	private ScreenView (double cX, double cY)  {
		ArenaScale aScl = ArenaScale.getArenaScaleObject();  // singleton
		svDvl = DebugLvl.getDebugLvlObject();

		adjustedScreenSizeX = aScl.adjustForScale (cX);  
		adjustedScreenSizeY = aScl.adjustForScale (cY);	
		
		screenSizeX = cX;  
		screenSizeY = cY;	

		screenCenter[0] = 0.0;  
		screenCenter[1] = 0.0;  
		
		svDvl.print (2, 0, "Screen View: scale is " + aScl.getArenaScale ());
		svDvl.print (2, 0, "     screen size x/y: " + screenSizeX + " : " + screenSizeY);
		svDvl.print (2, 0, "            adjusted: " + adjustedScreenSizeX + " : " + adjustedScreenSizeY);
		svDvl.print (2, 0, " ");
	}

	public static synchronized
					ScreenView getScreenViewObject()
	{
    	if (ref == null)
        	// it's ok, we can call this constructor
        	ref = new ScreenView(800, 600);
		return ref;
	}
	
	public static synchronized 
					ScreenView getScreenViewObject(double sX, double sY)
    {
    	if (ref == null)
        	// it's ok, we can call this constructor
        	ref = new ScreenView(sX, sY);
    	return ref;
    }

	public Object clone()
			throws CloneNotSupportedException
  	{
    	throw new CloneNotSupportedException(); // Don't allow cloning of a singleton
  	}

   public void setScreenSize (double sizeX, double sizeY)  { 
		ArenaScale aScl = ArenaScale.getArenaScaleObject();  // singleton

		adjustedScreenSizeX = aScl.adjustForScale (sizeX);  
		adjustedScreenSizeY = aScl.adjustForScale (sizeY);	
		
		screenSizeX = sizeX;  
		screenSizeY = sizeY;	
	}

	public void setScreenSize ()  {
		ArenaScale aScl = ArenaScale.getArenaScaleObject();  // singleton

		adjustedScreenSizeX = aScl.adjustForScale (screenSizeX);  
		adjustedScreenSizeY = aScl.adjustForScale (screenSizeY);	
	}
   
	public void setScreenCenter (double xC, double yC)  {
		screenCenter[0] = xC;  
		screenCenter[1] = yC;  		
	}
   
   // onScreen expects the values passed to be adjusted values for Arena Scale
   //  will return true if point passed in on screen, false if not
   //  left edge considered on screen, right edge off, top on, bottom off
   //
   public boolean onScreen (double[] tf) { 
   	boolean ret = false;
   	
		ArenaScale aScl = ArenaScale.getArenaScaleObject();  // singleton

		double aScale = aScl.getArenaScale();  

		svDvl.print (2, 0, " ");
		svDvl.print (2, 0, "onScreen");
		svDvl.print (2, 0, "Bot location: " + tf[0] + " : " + tf[1] + " : " + tf[2]);
		svDvl.print (2, 0, "ScreenSize: " + screenSizeX + " : " + screenSizeY);
		svDvl.print (2, 0, "Screen Center:  " + screenCenter[0] + " : " + screenCenter[1]);
		svDvl.print (2, 0, "Test value x1:  " + (screenCenter[0] - (adjustedScreenSizeX/2)));
		svDvl.print (2, 0, "Test value x2:  " + (screenCenter[0] + (adjustedScreenSizeX/2)));
		svDvl.print (2, 0, "Test value y1:  " + (screenCenter[1] - (adjustedScreenSizeY/2)));
		svDvl.print (2, 0, "Test value y2:  " + (screenCenter[1] + (adjustedScreenSizeY/2)));

		if ((tf[0] > (screenCenter[0] + ((adjustedScreenSizeX/2)*-1))) && (tf[0] < (screenCenter[0] + (adjustedScreenSizeX/2))))  {
			if ((tf[1] > (screenCenter[1] + ((adjustedScreenSizeY/2)*-1))) && (tf[1] < (screenCenter[1] + (adjustedScreenSizeY/2))))  {
				ret = true;
			}	
		}

		svDvl.print (2, 0, "onScreen returns: " + ret);
		svDvl.print (2, 0, " ");

   	return ret; 
   }

   public double screenPosX (double tf)  {
   	double ret = 0;
   	
		ArenaScale aScl = ArenaScale.getArenaScaleObject();  // singleton

     	ret = tf + adjustedScreenSizeX/2;
   	
   	return ret;
   }

   public double screenPosY (double tf)  {
   	double ret = 0;
   	
		ArenaScale aScl = ArenaScale.getArenaScaleObject();  // singleton

	   	ret = tf + adjustedScreenSizeY/2;
   	
   	return ret;
   }
}
	 