import java.io.*;
import java.lang.*;
import java.util.*;

public class MsgParser {

	private DebugLvl parser_dbug;
	private String[] parsedStr = null;
	private static MsgParser ref;

	private MsgParser ()  {
		parser_dbug = DebugLvl.getDebugLvlObject();    // singleton
	}

	public static synchronized 
					MsgParser getParserObject()
    {
    	if (ref == null)
        	// it's ok, we can call this constructor
        	ref = new MsgParser();
    	return ref;
    }

	public Object clone()
			throws CloneNotSupportedException
  	{
    	throw new CloneNotSupportedException(); // Don't allow cloning of a singleton
  	}
  	
	public String[] parseMsg (String inStr)  { 
  		if (inStr != null)  {
  			parsedStr = inStr.split("[,]+");
  		}  else  {
  			parsedStr = null;
  		}

		return (parsedStr); 
	}
}
