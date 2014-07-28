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

/*
import java.io.*;
import java.lang.*;
import java.util.*;

public class DataQueue implements Runnable {

	private MsgStub txtFile;
	private int botId;
	private int qCnt = 0;
	private DebugLvl dataq_dbug;

	private Queue<String> strQueue = new LinkedList<String>();
	private String[] currentStr = null;
	private int qMax = 100;

	public DataQueue (String feedFile, int bid, BotContainer bC)  {
		dataq_dbug = DebugLvl.getDebugLvlObject();    // singleton
		
		botId = bid;
		
		txtFile = new MsgStub();
	}
	
	public void run ()  { 
		String queueStr = null;
		int kk = 0;
		
		do  {
			queueStr = txtFile.getNextDataLine();
			dataq_dbug.print (1, kk, "queueStr: " + queueStr);
			
			if (queueStr != null)  {
				dataq_dbug.print(2, botId, ":" + kk + ": Adding [" + queueStr + "] to queue"); 
				if (strQueue.add (queueStr))  { 
					++qCnt;
					++kk;
				}  else  {
					dataq_dbug.print(2, botId, "Queue Add failed"); 
				}
			}
		}  while (queueStr != null && kk < qMax);
	}


	/*  This method uses poll which returns null if queue is empty
	 *  This is needed to set currentStr for getCurrData method to work properly
	 */
/*	public String[] getNextData ()  { 
		String qStr = null;

		dataq_dbug.print(1, botId, "qCnt is [" + qCnt + "]"); 
		
		if (qCnt > 0)  {
			qStr = strQueue.remove();
			qCnt--;
			dataq_dbug.print(1, botId, "qStr is [" + qStr + "]"); 
		}
		
  		if (qStr != null)  {
  			currentStr = qStr.split("[,]+");
  		}

		dataq_dbug.print(1, botId, "currentStr is : " + currentStr[0] + " " + currentStr[1]); 

		return (currentStr); 
	}

	/*  This method uses poll which returns null if queue is empty
	 *  This is needed to set currentStr for getCurrData method to work properly
	 */
/*
	public String[] waitForNextData ()  { 
		String qStr = null;

		dataq_dbug.print(3, botId, "qCnt is [" + qCnt + "]"); 
		
		while (qCnt <= 0) ;
		
		if (qCnt > 0)  {
			qStr = strQueue.remove();
			qCnt--;
			dataq_dbug.print(1, botId, "qStr is [" + qStr + "]"); 
		}
		
  		if (qStr != null)  {
  			currentStr = qStr.split("[,]+");
  		}  else  {
  			currentStr = null;
  		}

		return (currentStr); 
	}

    /*  This method returns the latest data retrieved from queue without
     *   having to hit queue again.
     */ 
//	public String[] getCurrData ()  { return (currentStr); }
//}
