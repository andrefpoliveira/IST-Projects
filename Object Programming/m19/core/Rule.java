package m19.core;

import java.io.Serializable;
import m19.core.exception.RuleFailed;


public abstract class Rule implements Serializable{
	private static final long serialVersionUID = 201901101343L;
	
	private int _id;

	public Rule(int id){
		_id = id;
	}

	public int getId() {
		return _id;
	}

	public abstract void checkRule(User user, Work work) throws RuleFailed;
}