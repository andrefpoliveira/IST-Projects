package m19.core;

import java.util.ArrayList;
import java.io.Serializable;

public abstract class Subject implements Serializable{
	protected ArrayList<Observer> _observers;

	public Subject(){
		_observers = new ArrayList<Observer>();
	}

	public abstract void update(String message);

	public void add(Observer observer){
		_observers.add(observer);
	}

	public void remove(Observer observer){
		_observers.remove(observer);
	}
}
