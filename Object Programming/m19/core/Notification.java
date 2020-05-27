package m19.core;

import java.util.ArrayList;


public abstract class Notification extends Subject{
	protected String _message;
	
	public abstract void update(String message);

	public String getMessage(){
		return _message;
	}
}
