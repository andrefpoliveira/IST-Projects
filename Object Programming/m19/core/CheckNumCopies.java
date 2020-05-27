package m19.core;

import m19.core.exception.RuleFailed;

public class CheckNumCopies extends Rule{
	//private static final long serialVersionUID = 201901150000L;
	public CheckNumCopies(int id){
		super(id);
	}

 @Override
	public void checkRule(User user, Work work) throws RuleFailed{
		if(work.getNumCopiesAvailable() == 0)
			throw new RuleFailed(user.getId(), work.getId(), getId());
	}
}
