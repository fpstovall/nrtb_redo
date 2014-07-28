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


import javax.swing.*;
import java.awt.*;
import java.awt.event.*;


public class CtrOnActionListener implements ActionListener  {
	private Canvas canv;
	private double zaX, zaY;
	private Robot bCont;
		
	public CtrOnActionListener (Canvas cv, Robot ctrBot, double xs, double xy)  {
		canv = cv;
		zaX = xs;
		zaY = xy;
		bCont = ctrBot;
	}
	
  	public void actionPerformed(ActionEvent e) {
  	 	String ae = e.getActionCommand();

  		ScreenView gsv = ScreenView.getScreenViewObject();
   		
		if (ae.equals("Bot"))  {
			canv.setCenterDot (400, 300);	
			gsv.setScreenCenter (400, 300);
		}
		if (ae.equals("0,0"))  {
			canv.setCenterDot (0, 0);	
			gsv.setScreenCenter (0, 0);
		}
		
   	canv.canvasPaint();
   }

}
