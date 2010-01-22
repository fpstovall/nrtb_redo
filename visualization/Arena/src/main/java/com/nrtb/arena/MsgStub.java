
import java.io.*;
import java.lang.*;
import java.lang.Math;
import java.util.Random;

public class MsgStub {

	private Random posNum = new Random();
	private DebugLvl mStub_dbug;

	private BufferedReader dataFile;
	private String[] strParts = null;
	private boolean firsTime = true;
	
	private double xMove = 0;
	private double yMove = 0;

	private double xP, yP;
	
	public MsgStub ()  {
		mStub_dbug = DebugLvl.getDebugLvlObject();    // singleton
	}

	public String getNextDataLine ()  {
		String str = null;
		int xSign = 1;
		int ySign = 1;

		xMove = Math.round(posNum.nextDouble() * 10);
		yMove = Math.round(posNum.nextDouble() * 10);

		if (Math.round(posNum.nextDouble() * 10) <= 5.0)  {
			xSign = -1;
		}

		if (Math.round(posNum.nextDouble() * 10) > 5.0)  {
			ySign = -1;
		}

		if (firsTime)  {
	  		xP = Math.round(posNum.nextDouble() * (posNum.nextDouble() * 10.0) * 100.0) * xSign;
	  		yP = Math.round(posNum.nextDouble() * (posNum.nextDouble() * 10.0) * 100.0) * ySign;

	  		str = "" + xP + ", " + yP + ", 0";
	  		 
			firsTime = false;
		
		}  else if (Math.round(posNum.nextDouble() * 10) <= 5.0)  {
	  		xP += (xMove * xSign); 	
	  		str = "" + xP + ", " + yP + ", 0"; 
		
		}  else  {
	  		yP += (yMove * xSign); 
	  		str = "" + xP + ", " + yP + ", 0"; 
		}

		mStub_dbug.print (1, 0, " ");
  		mStub_dbug.print (1, 0, "MsgStub:");
  		mStub_dbug.print (1, 0, "   xP: " + xP + "  yP: " + yP);
  		mStub_dbug.print (1, 0, "   str: " + str);
  		mStub_dbug.print (1, 0, " ");

		return (str);	
	}

	public double getNextDirection (double inDir)  {
		double ret;
		
		ret = Math.round(posNum.nextDouble() * 10);
		
		if (ret <= 5.0)  {
			ret += inDir;
		}  else  {
			ret = inDir;
		}

		return (ret);
	}
}
	 