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
