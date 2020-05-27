package m19.core;

public class RequestNotification extends Notification{

	public void update(String message){
		_message = message;
		for(Observer observer : _observers)
			observer.update(this);
	}

	@Override
	public String getMessage(){
		return "REQUISIÇÃO: " + _message;
	}
}
