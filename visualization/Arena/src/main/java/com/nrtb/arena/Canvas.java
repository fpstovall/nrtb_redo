import javax.swing.*;

import java.awt.*;
import java.awt.geom.*;
import java.awt.event.*;

import java.io.*;
import java.lang.*;
import java.util.*;

import java.awt.MenuItem;
import java.awt.PopupMenu;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;


public class Canvas extends JPanel
	        implements MouseListener,MouseMotionListener  {

	private java.awt.Graphics2D g2;
	final JPopupMenu menu = new JPopupMenu();
	
	private ExitNow xN;
	
	private ArenaScale aScl;
	private ScreenView scrView;
	private Vector<Robot> rbots;
	private CenterDot cDot = new CenterDot();
	private DebugLvl canvas_dbug;
	
   public Canvas(Vector<Robot> bots) {
    	rbots = bots;

    	canvas_dbug = DebugLvl.getDebugLvlObject();    // singleton
    	aScl = ArenaScale.getArenaScaleObject();     // singleton
		scrView = ScreenView.getScreenViewObject();   // singleton

		xN = new ExitNow("Exit", null, "Exit the program", new Integer(KeyEvent.VK_X));	
    	
    	JMenuItem item = new JMenuItem(xN);
		menu.add(item);

      //Create horizontal scrollbar.
      Scrollbar horz = new Scrollbar(Scrollbar.HORIZONTAL);
      horz.setValue(50);

      //Create vertical scrollbar.
      Scrollbar vert = new Scrollbar(Scrollbar.VERTICAL);
      vert.setValue(50);

      //Add Scrollbars
      setLayout(new BorderLayout());
      add("East", vert);
      add("South", horz);

      addMouseListener(this);
      addMouseMotionListener(this);
   	setBackground(Color.white);
	}

	public void paintComponent(java.awt.Graphics g) {
      //because we extend JPanel, this erases the background
      //with the background color
      super.paintComponent(g);

		canvas_dbug.print (1, 0, "paintComponent: ");
				                              		
		g2 = (java.awt.Graphics2D) g;

		g2.setRenderingHint(java.awt.RenderingHints.KEY_ANTIALIASING,
		 					java.awt.RenderingHints.VALUE_ANTIALIAS_ON);

		g2.scale(aScl.getArenaScale(),aScl.getArenaScale());

		if (scrView.onScreen (cDot.getLocation()))  {
			cDot.draw(g2);
		}

		// Check all bots for visiblity and draw if on screen
		Iterator itr = rbots.iterator(); 
		while (itr.hasNext())  {
			Robot rbt = (Robot) itr.next();
			
			if (scrView.onScreen (rbt.getLocation()))  {
				rbt.draw(g2);
			}
		}				
	}

	public String setBotPopupInfo (Robot bb)  {
		String str = null;
		
		str = "Location : " + (bb.getLocation())[0] + "," + (bb.getLocation())[1];
				
		return (str);
	}

	public void canvasPaint ()  { repaint(); }
	
	public void setCenterDot (double xC, double yC)  { 
		double[] newD = { xC, yC, 0 };
		
		cDot.setLocation (newD); 
	}
		
   public void mouseClicked(MouseEvent e) {
    	// need orginal mouse coords for popup location
    	int ptXOrig = e.getX();
    	int ptYOrig = e.getY();

    	//
		// since mouse coords are reported in orginal point system regardless of scale
		//   we adjust the values reported for current scale
		//
    	int ptX = (new Double(aScl.adjustForScale(ptXOrig))).intValue();
    	int ptY = (new Double(aScl.adjustForScale(ptYOrig))).intValue();
    	
    	double[] newBotData = new double[3];
    	Panel pan1 = new Panel();
		boolean doneIt = false;
    	Iterator itr;
    	
    	canvas_dbug.print (2, 0, "Orig   Mouse values : " + ptXOrig + "  " + ptYOrig);
    	canvas_dbug.print (2, 0, "Scaled Mouse values : " + ptX + "  " + ptY);
    	canvas_dbug.print (2, 0, "  ");
    	
    	switch (e.getButton())  {
    	case 1:
				// Move all the robots
				itr = rbots.iterator(); 
				while (itr.hasNext())  {((Robot) itr.next()).moveBotNextPos();	}

    			break;
    			
    	case 2:
    			break;
    			
    	case 3:
				doneIt = false;
					
				// Reset iterator to work thru all the robots
				itr = rbots.iterator(); 
				while (itr.hasNext())  {
					Robot rbt = (Robot) itr.next();
					
					double[] botRealPts = rbt.getLocation ();
					double[] botScrPts = {scrView.screenPosX(botRealPts[0]),scrView.screenPosY(botRealPts[1])};
					botScrPts[0] -= Robot.centerOffset_W;
					botScrPts[1] -= Robot.centerOffset_H;

					// check to see if we are inside of a bot or not
					// size of the width and height of the bot adjusted for current scale
					//
					if (ptX >= botScrPts[0] && ptX <= botScrPts[0] + Robot.CHASSIS_W)  {
						if (ptY >= botScrPts[1] && ptY <= botScrPts[1] + Robot.CHASSIS_H)  {
							String bStr = setBotPopupInfo((rbt));
							new PopupBotInfo ("Bot Id: " + Double.toString(rbt.getId()), bStr).setVisible(true);
							doneIt = true;
						}
					}
				}

				if (!doneIt) { 
					menu.show(e.getComponent(), ptXOrig, ptYOrig); 
				}
    			break;

    	default:	
    			break;
    	}
		repaint();					
    }
	
    //not needed
    public void mouseExited(MouseEvent e) {}
    public void mouseEntered(MouseEvent e) {}
    public void mouseMoved(MouseEvent e) {}
    public void mouseDragged(MouseEvent e) {}

    public void mouseReleased(MouseEvent e) {
   
    	switch (e.getButton())  {
    	case 1:
				this.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
				break;
    	case 2:
    			canvas_dbug.print(1, 0, "Button 2 clicked");
    			break;
    	case 3:
    			canvas_dbug.print(1, 0, "Button 3 clicked");
    			break;

    	default:	
    			canvas_dbug.print(1, 0, "Don't know which Button clicked");
    			break;
    	}
		repaint();					
	}

   public void mousePressed(MouseEvent e) {
    	
    	switch (e.getButton())  {
    	case 1:
				this.setCursor(Cursor.getPredefinedCursor(Cursor.MOVE_CURSOR));
				break;
    	case 2:
    			canvas_dbug.print(1, 0, "Button 2 clicked");
    			break;
    	case 3:
    			canvas_dbug.print(1, 0, "Button 3 clicked");
    			break;

    	default:	
    			canvas_dbug.print(1, 0, "Don't know which Button clicked");
    			break;
    	}
		repaint();					
	}		
}
