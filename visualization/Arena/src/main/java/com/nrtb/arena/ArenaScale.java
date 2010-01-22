import javax.swing.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.event.*;
import java.io.*;
import java.lang.*;
 
public class ArenaScale {

	private double scl;
   private static ArenaScale ref;
	
	private ArenaScale () {
		scl = .50;
	}

	public static synchronized 
					ArenaScale getArenaScaleObject()
    {
    	if (ref == null)
        	// it's ok, we can call this constructor
        	ref = new ArenaScale();
    	return ref;
    }

	public Object clone()
			throws CloneNotSupportedException
  	{
    	throw new CloneNotSupportedException(); // Don't allow cloning of a singleton
  	}
  
	public double getArenaScale ()  { return (scl); }

	public void setArenaScale (double nscl)  {
		if (nscl < 0)  {
			scl = 0;
		}  else  {
			scl = nscl;
		}
	}

	public void adjustArenaScaleUp (int bNum)  {
		for (int ii = 0; ii < bNum; ii++)  {
			if (scl < 2.00)  scl += .10;
		}
	}

	public void adjustArenaScaleDown (int bNum)  {
		for (int ii = 0; ii < bNum; ii++)  {
			if (scl >= 0.30)  {	scl -= .10;	}
		}
	}
	
	public double adjustForScale (double val)  {
		Double ret = new Double(val);
		
		if (scl < 1)  {
			ret = val / scl;
		}  else if (scl > 1)  {
		    ret = scl * val;
		}
		    
		return (ret.doubleValue());	
	}
}