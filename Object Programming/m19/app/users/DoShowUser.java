package m19.app.users;

import m19.app.exception.NoSuchUserException;
import m19.core.LibraryManager;
import m19.core.exception.InvalidUserIdNumber;
import m19.core.exception.InvalidWorkIdNumber;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;
// FIXME import other core concepts
// FIXME import other ui concepts

/**
 * 4.2.2. Show specific user.
 */
public class DoShowUser extends Command<LibraryManager> {

  private Input<Integer> _id;

  /**
   * @param receiver
   */
  public DoShowUser(LibraryManager receiver) {
    super(Label.SHOW_USER, receiver);
    _id = _form.addIntegerInput(Message.requestUserId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();

    try {
      _display.addLine(_receiver.showUser(_id.value()).getDescription());
      _display.display();
    } catch (InvalidUserIdNumber iin){
      throw new NoSuchUserException(_id.value());
    }
    
    
  }

}
