/**  	Java Implementation: NRTB Arena Class
 *
 *   	sccs: bazaar:     lp:nrtb
 *  
 *  	Description:
 *
 *
 *
 *  	@Author: George Jordan,  (C) 2009
 */
 
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JMenuBar;
import javax.swing.JComboBox;
import javax.swing.JButton;
import javax.swing.Box;
import javax.swing.JScrollPane;

import java.awt.*;
import java.awt.geom.*;
import java.awt.event.*;

import java.io.*;
import java.lang.*;

import java.awt.MenuItem;
import java.awt.PopupMenu;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;

import java.util.Vector;

 
public class Arena {
	private int bNum = 0;                                 // num of bots in arena
	private BufferedReader arenaDataFile;		          // data file containing arena parameters
	private String[] arenaFeedParts = new String[1];      // arena parameters data holder

   private static Vector<Robot> bots; //coordinates used to draw graphics
   
  	private JMenuBar menuBar;
	private JMenu zoom, ctrOn;
	
	private JFrame frame = new JFrame("Arena");           // Arena frame 
	private Canvas canvas;
	private ScreenView sView;
	private DebugLvl dLvl;
	
	private double arenaSizeX = 900;
	private double arenaSizeY = 700;
	private double screenSizeX = arenaSizeX - 100;
	private double screenSizeY = arenaSizeY - 100;


   public static void main(String[] args) {
		final int dbugLevel;
    	
    	if (args.length > 0)
    		dbugLevel = (new Integer(args[0])).intValue();
    	else
    		dbugLevel = 0;

		// initialize singletons
		ArenaScale.getArenaScaleObject();
    	DebugLvl.getDebugLvlObject(dbugLevel);

      bots = new Vector<Robot>(50);

    	EventQueue.invokeLater(new Runnable() {
            public void run() {
                new Arena();
            }
      });
	}
    
   public Arena() {
      String str = null;
      dLvl = DebugLvl.getDebugLvlObject();
        	
		dLvl.print(1, 0, "Debug Level set to: " + dLvl.levelIs());

		// Get number of bots from config file
		//
  		try {
			arenaDataFile = new BufferedReader(new FileReader("arena_feed.txt"));

   		str = arenaDataFile.readLine();
    
   		if (str != null)  {
				arenaFeedParts = str.split("[,]+");
				
				bNum = Integer.parseInt(arenaFeedParts[0]);		
	   	}
   	
   	} catch (IOException e) {
   	}  

		// Set up bot objects
		//
		dLvl.print(1, 0, "Number of bots: " + bNum);
		
		for (int ii = 1; ii <= bNum; ii++)  {
			bots.addElement( new Robot(ii) );
		}

		// Build arena frame
		//
    	frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		// Set up Canvas to use within frame to draw on
		//
		canvas = new Canvas(bots);
		Cursor canvasCursor = canvas.getCursor();
		canvas.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
		
		// Must create screen view singleton now to prevent problems later
		//
      sView = ScreenView.getScreenViewObject(screenSizeX, screenSizeY);

		// menu build/setup must be done AFTER canvas set up for zoom menu items 
		// to call canvas repaint correctly
		//
		menuBar = new JMenuBar();

      zoom = new JMenu (" Scale ");
      zoom.setMnemonic(KeyEvent.VK_S);

		ScaleActionListener zal = new ScaleActionListener(canvas, arenaSizeX, arenaSizeY);

		JMenuItem z5   = new JMenuItem("5");		zoom.add (z5);	   	z5.addActionListener(zal);
		JMenuItem z10  = new JMenuItem("10");		zoom.add (z10);		z10.addActionListener(zal);
		JMenuItem z15 	= new JMenuItem("15");		zoom.add (z15);		z15.addActionListener(zal);
		JMenuItem z25  = new JMenuItem("25");		zoom.add (z25);		z25.addActionListener(zal);
		JMenuItem z50 	= new JMenuItem("50");		zoom.add (z50);		z50.addActionListener(zal);
		JMenuItem z75 	= new JMenuItem("75");		zoom.add (z75);		z75.addActionListener(zal);
		JMenuItem z100 = new JMenuItem("100");		zoom.add (z100);		z100.addActionListener(zal);

		menuBar.add(zoom);

		menuBar.add( new Box.Filler(new Dimension(700, 2), new Dimension(700, 2), new Dimension(700, 2)));
		
		frame.setJMenuBar(menuBar);

		// Put canvas in frame and rest of frame setup
		//
    	frame.setContentPane(canvas);

		frame.setResizable (false);
		frame.setTitle("NRTB Visualization (Prototype): ");
      frame.setSize((int)arenaSizeX,(int)arenaSizeY);
      frame.setVisible(true);
   }
}
