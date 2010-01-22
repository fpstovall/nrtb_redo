
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
