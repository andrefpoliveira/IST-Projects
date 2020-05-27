package m19.core;

import java.io.Serializable;

public class BehaviorNormal implements UserBehavior{
  private static final long serialVersionUID = 201901600002L;

 @Override
	public int getRequestSize(){
		return 3;
	}

 @Override
	public String getBehavior(){
		return "NORMAL";
	}

 @Override
	public int getDeadline(int numCopies){
		if(numCopies > 5)
			return 15;
		else if(numCopies > 1)
			return 8;
		else
			return 3;
	}

	public int checkNumReturn(int date, int workDeadline, User user){
		if(date >= workDeadline)
			user.setNumReturnFail();
		else {
			user.setNumReturnSuccess();
		}
		if(user.getNumReturn() == -3){
			user.setFaltoso();
		}
		else if(user.getNumReturn() == 5){
			user.setCumpridor();
		}
		return date - workDeadline;
	}
}
