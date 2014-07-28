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


import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;

public class PopupBotInfo extends Frame {
  /** Construct the main program */
  public PopupBotInfo(String title, String info) {
    super(title);

	Button cls = new Button("Close");

	class BCls implements ActionListener {
      public void actionPerformed(ActionEvent e) {
      	setVisible(false);
      }
    }
    cls.addActionListener(new BCls());

    setLayout(new FlowLayout());
    
    add(new TextArea (info));
    add(cls);
    
    pack();
    setVisible(true);
  }
}
