package m19.core;

import java.io.Serializable;

public class BehaviorFaltoso implements UserBehavior{
  private static final long serialVersionUID = 201901600001L;

 @Override
	public int getRequestSize(){
		return 1;
	}

 @Override
	public String getBehavior(){
		return "FALTOSO";
	}

 @Override
	public int getDeadline(int numCopies){
		return 2;
	}

	public int checkNumReturn(int date, int workDeadline, User user){
		if(date >= workDeadline)
			user.setNumReturnFail();
		else {
			user.setNumReturnSuccess();
		}
		if(user.getNumReturn() == 3){
			user.setNormal();
		}
		return date - workDeadline;
	}
}
