package m19.core;

import java.io.Serializable;

public class BehaviorCumpridor implements UserBehavior{
  private static final long serialVersionUID = 20190160000L;

 @Override
	public int getRequestSize(){
		return 5;
	}

 @Override
	public String getBehavior(){
		return "CUMPRIDOR";
	}

 @Override
	public int getDeadline(int numCopies){
		if(numCopies > 5)
			return 30;
		else if(numCopies > 1)
			return 15;
		else
			return 8;
	}

	public int checkNumReturn(int date, int workDeadline,User user){
		if(date >= workDeadline)
			user.setNumReturnFail();
		else {
			user.setNumReturnSuccess();
		}
		if(user.getNumReturn() == -3){
			user.setFaltoso();
		}
		return date - workDeadline;
	}
}
