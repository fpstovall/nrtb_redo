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


import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.lang.*;
 
abstract class ArenaObject {

	// defines
	public final static int WIDTH = 0;
	public final static int LENGTH = 1;
	public final static int HEIGHT = 2;	

	public final static int locX = 0;
	public final static int locY = 1;
	public final static int locZ = 2;

	// variables
	public double[] location;          	// location of object
	public double[] wlh;						// width, length and height of object
	public double id;                   // object id
	
	// methods
	public void setId (double newId)  { id = newId; }
	public double getId () { return id; }
	
	public void setLocation (double[] newLoc)  {	location = newLoc; }
	public double[] getLocation () { return location; }

	public void setWLH (double[] newWLH)  { wlh = newWLH; }
	public double[] getWLH () { return wlh; }

	// abstract methods
	abstract void draw(java.awt.Graphics g);
}