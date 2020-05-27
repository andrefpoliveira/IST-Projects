package m19.app.users;

import java.util.List;

import m19.core.LibraryManager;
import m19.core.User;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;

/**
 * 4.2.4. Show all users.
 */
public class DoShowUsers extends Command<LibraryManager> {

  /**
   * @param receiver
   */
  public DoShowUsers(LibraryManager receiver) {
    super(Label.SHOW_USERS, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    List<User> arr = _receiver.showUsers();
    for (User u : arr) {
      _display.addLine(u.getDescription());
    }
    _display.display();
  }
  
}
