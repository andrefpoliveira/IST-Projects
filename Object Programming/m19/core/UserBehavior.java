package m19.core;

import java.io.Serializable;

public interface UserBehavior extends Serializable {
   //private static final long serialVersionUID = 201901600004L;

   int getRequestSize();
   String getBehavior();
   int getDeadline(int numCopies);
   int checkNumReturn(int date, int workDeadline, User user);
}
