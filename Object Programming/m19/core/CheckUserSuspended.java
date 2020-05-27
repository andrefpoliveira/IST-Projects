package m19.core;

import m19.core.exception.RuleFailed;

public class CheckUserSuspended extends Rule {
	//private static final long serialVersionUID = 201901150004L;
	public CheckUserSuspended(int id){
		super(id);
	}

 @Override
	public void checkRule(User user, Work work) throws RuleFailed {
		if(!user.isActive())
			throw new RuleFailed(user.getId(), work.getId(), getId());
	}
}
