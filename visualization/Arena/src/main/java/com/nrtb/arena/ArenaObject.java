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