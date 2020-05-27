package m19.core;

import m19.core.exception.RuleFailed;

public class CheckWorkPrice extends Rule {
	//private static final long serialVersionUID = 201901150005L;

	public CheckWorkPrice(int id){
		super(id);
	}
    @Override
	public void checkRule(User user, Work work) throws RuleFailed{
		if(work.getPrice() > 25 && user.getBehavior().getBehavior() != "Cumpridor")
			throw new RuleFailed(user.getId(), work.getId(), getId());
	}
}
