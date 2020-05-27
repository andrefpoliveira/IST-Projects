package m19.app.users;

import m19.app.exception.NoSuchUserException;
import m19.core.LibraryManager;
import m19.core.exception.InvalidUserIdNumber;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;

import java.util.List;

import m19.core.Notification;

/**
 * 4.2.3. Show notifications of a specific user.
 */
public class DoShowUserNotifications extends Command<LibraryManager> {

  private Input<Integer> _id;

  /**
   * @param receiver
   */
  public DoShowUserNotifications(LibraryManager receiver) {
    super(Label.SHOW_USER_NOTIFICATIONS, receiver);
    _id = _form.addIntegerInput(Message.requestUserId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try{
	    List<Notification> notifications = _receiver.showUser(_id.value()).getNotifications();
	    for(Notification notification : notifications)
	      _display.addLine(notification.getMessage());
	    _display.display();
	    _receiver.removeUserNotification(_id.value());
     } catch (InvalidUserIdNumber iin){
	throw new NoSuchUserException(_id.value());
	}
  }

}
