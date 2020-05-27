package m19.app.users;

import m19.app.exception.NoSuchUserException;
import m19.app.exception.UserIsActiveException;
import m19.core.LibraryManager;
import m19.core.exception.InvalidUserIdNumber;
import m19.core.exception.UserIsActive;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;

/**
 * 4.2.5. Settle a fine.
 */
public class DoPayFine extends Command<LibraryManager> {

  private Input<Integer> _id;

  /**
   * @param receiver
   */
  public DoPayFine(LibraryManager receiver) {
    super(Label.PAY_FINE, receiver);
    _id = _form.addIntegerInput(Message.requestUserId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();

    try {
      _receiver.payFine(_id.value());
    } catch (UserIsActive uia) {
      throw new UserIsActiveException(_id.value());
    } catch (InvalidUserIdNumber iuid) {
      throw new NoSuchUserException(_id.value());
    }
  }

}
