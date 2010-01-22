
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
