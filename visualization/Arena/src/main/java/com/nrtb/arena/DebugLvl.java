
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
	 