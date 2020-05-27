package m19.core;

import m19.core.exception.RuleFailed;

public class CheckReferenceWork extends Rule {
	//private static final long serialVersionUID = 201901150002L;
	public CheckReferenceWork(int id){
		super(id);
	}

 @Override
	public void checkRule(User user, Work work) throws RuleFailed{
		if(work.getCategory().toString() == "Referência")
			throw new RuleFailed(user.getId(), work.getId(), getId());
	}
}
