package m19.core;

import java.io.Serializable;

public class Request implements Serializable{
	private static final long serialVersionUID = 201901600007L;
	private int _deadline;

	private Work _work;

	private User _user;

	public Request(int deadline, Work work, User user){
		_deadline = deadline;
		_work = work;
		_user = user;
	}

	public Work getWork(){
		return _work;
	}

	public int getDeadline() {
		return _deadline;
	}
}
