package m19.app.users;

import m19.core.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;
import m19.app.exception.UserRegistrationFailedException;
import m19.core.exception.FailedToRegisterUser;

/**
 * 4.2.1. Register new user.
 */
public class DoRegisterUser extends Command<LibraryManager> {

  private Input<String> _name;
  private Input<String> _email;

  /**
   * @param receiver
   */
  public DoRegisterUser(LibraryManager receiver) {
    super(Label.REGISTER_USER, receiver);
    _name = _form.addStringInput(Message.requestUserName());
    _email = _form.addStringInput(Message.requestUserEMail());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try {
      _display.addLine(Message.userRegistrationSuccessful(_receiver.registerUser(_name.value(),_email.value())));
      _display.display();
    } catch (FailedToRegisterUser ftru) {
      throw new UserRegistrationFailedException(_name.value(), _email.value());
    }

    
  }

}
