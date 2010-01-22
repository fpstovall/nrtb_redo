
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
