package m19.app.requests;

import m19.app.exception.*;
import m19.core.LibraryManager;
import m19.core.exception.*;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
// FIXME import other core concepts
// FIXME import other ui concepts

/**
 * 4.4.2. Return a work.
 */
public class DoReturnWork extends Command<LibraryManager> {

  private Input<Integer> _userId;
  private Input<Integer> _workId;
  private Input<Boolean> _paymentChoice;
  private int _fine;

  /**
   * @param receiver
   */
  public DoReturnWork(LibraryManager receiver) {
    super(Label.RETURN_WORK, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _userId = _form.addIntegerInput(Message.requestUserId());
    _workId = _form.addIntegerInput(Message.requestWorkId());
    _form.parse();
    _form.clear();

    try {
      _fine = _receiver.returnWork(_userId.value(), _workId.value());

      if(_fine > 0) {
        _display.popup(Message.showFine(_userId.value(), _fine));
        _paymentChoice = _form.addBooleanInput(Message.requestFinePaymentChoice());
        _form.parse();
        _form.clear();

        if (_paymentChoice.value()) {
          _receiver.payFine(_userId.value());
          _receiver.checkUserIsOutOfDate(_userId.value());
        }
      }

    } catch(MissingFileAssociationException mfse) {
      throw new WorkNotBorrowedByUserException(_workId.value(), _userId.value());
    } catch (InvalidUserIdNumber iuid) {
      throw new NoSuchUserException(_userId.value());
    } catch (InvalidWorkIdNumber iwid) {
      throw new NoSuchWorkException(_workId.value());
    } catch (UserIsActive uia) {
      throw new UserIsActiveException(_userId.value());
    }
  }

}
