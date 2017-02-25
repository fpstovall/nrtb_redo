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
import java.awt.event.*;


public class ScaleActionListener implements ActionListener  {
	private Canvas canv;
	private double zaX, zaY;
	
	public ScaleActionListener (Canvas cv, double xs, double xy)  {
		canv = cv;
		zaX = xs;
		zaY = xy;
	}
	
  	public void actionPerformed(ActionEvent e) {
  	  		Double ae = new Double(e.getActionCommand());
   		ArenaScale as = ArenaScale.getArenaScaleObject();
   		ScreenView gsv = ScreenView.getScreenViewObject(zaX, zaY);
   		
   		as.setArenaScale (ae.doubleValue() / 100.0);
   		gsv.setScreenSize ();
   		
	   	canv.canvasPaint();
   	}  
}
