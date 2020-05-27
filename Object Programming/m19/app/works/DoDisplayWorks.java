package m19.app.works;

import m19.core.LibraryManager;
import m19.core.Work;
import java.util.List;
// FIXME import other core concepts
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Command;
// FIXME import ui concepts

/**
 * 4.3.2. Display all works.
 */
public class DoDisplayWorks extends Command<LibraryManager> {

  /**
   * @param receiver
   */
  public DoDisplayWorks(LibraryManager receiver) {
    super(Label.SHOW_WORKS, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    List<Work> arr = _receiver.showWorks();
    for (Work w : arr) {
      _display.addLine(w.getDescription());
    }
    _display.display();
  }
}
