package m19.core;

import java.io.Serializable;

public interface Observer extends Serializable{

	public abstract void update(Notification notification);

}
