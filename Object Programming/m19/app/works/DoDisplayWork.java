package m19.app.works;

import m19.core.LibraryManager;
import pt.tecnico.po.ui.DialogException;
import m19.core.exception.InvalidWorkIdNumber;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.Command;
import m19.app.exception.NoSuchWorkException;

/**
 * 4.3.1. Display work.
 */
public class DoDisplayWork extends Command<LibraryManager> {

  private Input<Integer> _id;

  /**
   * @param receiver
   */
  public DoDisplayWork(LibraryManager receiver) {
    super(Label.SHOW_WORK, receiver);
    _id = _form.addIntegerInput(Message.requestWorkId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();

    try {
      _display.addLine(_receiver.showWork(_id.value()).getDescription());
      _display.display();
    } catch (InvalidWorkIdNumber iin){
      throw new NoSuchWorkException(_id.value());
    } 
  }
  
}
