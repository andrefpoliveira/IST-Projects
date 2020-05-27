package m19.core;

public class ReturnNotification extends Notification{

	public void update(String message){
		_message = message;
		for(Observer observer : _observers)
			observer.update(this);
	}

	@Override
	public String getMessage(){
		return "ENTREGA: " + _message;
	}
}
