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
 * 4.4.1. Request work.
 */
public class DoRequestWork extends Command<LibraryManager> {

  private Input<Integer> _userId;

  private Input<Integer> _workId;

  private Input<Boolean> _notificationPreference;
  /**
   * @param receiver
   */
  public DoRequestWork(LibraryManager receiver) {
    super(Label.REQUEST_WORK, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _userId = _form.addIntegerInput(Message.requestUserId());
    _workId = _form.addIntegerInput(Message.requestWorkId());
    _form.parse();
    _form.clear();
    try{
      _display.popup(Message.workReturnDay(_workId.value(),_receiver.requestWork(_userId.value(), _workId.value())));
    } catch(RuleFailed rfe){
      if(rfe.getRuleID() == 3) {

        _notificationPreference = _form.addBooleanInput(Message.requestReturnNotificationPreference());
        _form.parse();
        _form.clear();

        if(_notificationPreference.value())
          _receiver.addUserNotification(_userId.value(),_workId.value());
      } else {
          throw new RuleFailedException(rfe.getUserID(), rfe.getWorkID(), rfe.getRuleID());
      }


    } catch (InvalidUserIdNumber e) {
      throw new NoSuchUserException(_userId.value());
    } catch (InvalidWorkIdNumber e) {
      throw new NoSuchWorkException(_workId.value());
    }
  }

}
