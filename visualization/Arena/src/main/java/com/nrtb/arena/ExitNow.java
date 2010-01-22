import javax.swing.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.event.*;
import java.io.*;
import java.lang.*;
//import target.classes.*;
import java.awt.MenuItem;
import java.awt.PopupMenu;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;

public class ExitNow extends AbstractAction {
	ArenaScale aScl;
	DebugLvl exitnow;
	
	public ExitNow (String text, ImageIcon icon, String desc, 
						Integer mnemonic) {
		super(text, icon);
   		putValue(SHORT_DESCRIPTION, desc);
   		putValue(MNEMONIC_KEY, mnemonic);
   		aScl = ArenaScale.getArenaScaleObject();   // singleton
   		exitnow = DebugLvl.getDebugLvlObject();    // singleton
   	}

   	public void actionPerformed(ActionEvent e) {
		System.exit(0);
   	}
}
