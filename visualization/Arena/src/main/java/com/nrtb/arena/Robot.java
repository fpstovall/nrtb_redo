/***********************************************
This file is part of the NRTB project (https://launchpad.net/nrtb).

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
 
 
public class Robot extends ArenaObject {

	private Rectangle2D.Double	chassis = new Rectangle2D.Double();
	private Ellipse2D.Double	phasedRadar = new Ellipse2D.Double();
	private Ellipse2D.Double	cannon = new Ellipse2D.Double();
	private Ellipse2D.Double	laser = new Ellipse2D.Double();


  	private BasicStroke stroke = new BasicStroke(1.5f);
  	private ScreenView scrView = null;
	private java.awt.Graphics2D g2;
	private MsgStub mStub = new MsgStub();

	public final static double centerOffset_W = 20;
	public final static double centerOffset_H = 25;

	private Color chassisColor = Color.GREEN;		
	private Color phasedColor = Color.RED;		
	private Color cannonColor = Color.BLUE;		
	private Color laserColor = Color.CYAN;			

	private double[] robotLoc = { 0, 0, 0 };
	private double robotDir = 360;                        // degrees

	// With the exception of the CHASSIS (rectangle) defines, 
	//    keep the width and height values the same to create
	//    a circle effect for the ellipses
	//
	public final static double CHASSIS_W = 40, CHASSIS_H = 50;
	public final static double PHASED_W  = 40, PHASED_H  = 40;
	public final static double CANNON_W  = 30, CANNON_H  = 30;
	public final static double LASER_W   = 10, LASER_H   = 10;
	
	
	public Robot (double Id)  {

		setId (Id);
		
		moveBotNextPos ();               // need to do this to get the bot off of 0,0

		setLocation (robotLoc);
		
		scrView = ScreenView.getScreenViewObject();
	}

	public Rectangle2D 	getChassis()  		{ return chassis; }
	public Ellipse2D		getPhasedRadar() 	{ return phasedRadar; }
  	public Ellipse2D 		getCannon()      	{ return cannon; }
  	public Ellipse2D 		getLaser()       	{ return laser; }

	public void moveBotNextPos()  {
		String str = mStub.getNextDataLine ();
		String[] strParts =	str.split("[,]+");
		
		robotLoc[0] = Double.parseDouble(strParts[0]); 	 
   	robotLoc[1] = Double.parseDouble(strParts[1]); 	 
	   robotLoc[2] = Double.parseDouble(strParts[2]);	

		setLocation (robotLoc);
	}
	 	  
	public void draw (java.awt.Graphics g)  {
		g2 = (java.awt.Graphics2D) g;

		g2.setStroke(stroke);
		
		g2.setColor(chassisColor);
	  	chassis.setRect(scrView.screenPosX(location[0] - centerOffset_W), 
   		 				 scrView.screenPosY(location[1] - centerOffset_H), 
   		 				 CHASSIS_W, CHASSIS_H);
		g2.rotate(Math.toRadians(robotDir));
		g2.fill(chassis);

		g2.setColor(phasedColor);
	   phasedRadar.setFrame(scrView.screenPosX(location[0] - centerOffset_W), 
   		 				 		scrView.screenPosY(location[1] + (5 - centerOffset_H)), 
   		 				 		PHASED_W, PHASED_H);
		g2.fill(phasedRadar);

		g2.setColor(cannonColor);
	   cannon.setFrame(scrView.screenPosX(location[0] + (5 - centerOffset_W)), 
   		 				 scrView.screenPosY(location[1] + (10 - centerOffset_H)), 
   		 				 CANNON_W, CANNON_H);
		g2.fill(cannon);

		g2.setColor(laserColor);
		laser.setFrame(scrView.screenPosX(location[0] + (15 - centerOffset_W)), 
   		 			   scrView.screenPosY(location[1] + (20 - centerOffset_H)), 
   		 				LASER_W, LASER_H);
		g2.fill(laser);
	}
}
