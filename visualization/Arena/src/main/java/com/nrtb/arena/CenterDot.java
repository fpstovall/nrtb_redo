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


import java.awt.Color;
import java.awt.EventQueue;
import java.awt.Graphics;
import java.awt.BasicStroke;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Rectangle2D;

import java.io.*;
import java.lang.*;
 
 
public class CenterDot extends ArenaObject {

	private Ellipse2D.Double	centerDot = new Ellipse2D.Double();


  	private BasicStroke stroke = new BasicStroke(1.5f);
	private java.awt.Graphics2D g2;
  	private ScreenView scrView = null;

	private double centerOffset_W = 20;
	private double centerOffset_H = 25;

	private Color dotColor = Color.RED;		

	private double[] centerLoc = { 0, 0, 0 };

	public final static double LASER_W  = 10, LASER_H  = 10;
	
	
	public CenterDot ()  {

		setLocation (centerLoc);
		
		scrView = ScreenView.getScreenViewObject();
	}

	public void draw (java.awt.Graphics g)  {
		g2 = (java.awt.Graphics2D) g;

		g2.setStroke(stroke);
		
		g2.setColor(dotColor);
	   centerDot.setFrame(scrView.screenPosX(location[0] + (15 - centerOffset_W)), 
   		 				 		scrView.screenPosY(location[1] + (20 - centerOffset_H)), 
   		 				 		LASER_W, LASER_H);
		g2.fill(centerDot);
	}
}
