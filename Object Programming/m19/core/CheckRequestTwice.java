package m19.core;

import java.util.List;
import m19.core.exception.RuleFailed;


public class CheckRequestTwice extends Rule {
	//private static final long serialVersionUID = 201901150003L;
	public CheckRequestTwice(int id){
		super(id);
	}

 @Override
	public void checkRule(User user, Work work) throws RuleFailed{
		List<Request> _requests = user.getRequests();
		for(Request request : _requests){
            if(request.getWork().equals(work))
                throw new RuleFailed(user.getId(), work.getId(), getId());
        }
	}
}
