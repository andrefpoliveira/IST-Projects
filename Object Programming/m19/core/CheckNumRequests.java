package m19.core;

import m19.core.exception.RuleFailed;

public class CheckNumRequests extends Rule {
	//private static final long serialVersionUID = 201901150001L;
	public CheckNumRequests(int id){
		super(id);
	}

 @Override
	public void checkRule(User user, Work work) throws RuleFailed{
		if(user.getRequests().size() >= user.getBehavior().getRequestSize())
			throw new RuleFailed(user.getId(), work.getId(), getId());
	}
}
