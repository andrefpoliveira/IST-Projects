package m19.app.works;

import m19.core.LibraryManager;
import java.util.List;
import m19.core.Work;
// FIXME import other core concepts
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
// FIXME import ui concepts

/**
 * 4.3.3. Perform search according to miscellaneous criteria.
 */
public class DoPerformSearch extends Command<LibraryManager> {

  private Input<String> _searchKey;

  /**
   * @param m
   */
  public DoPerformSearch(LibraryManager m) {
    super(Label.PERFORM_SEARCH, m);
    _searchKey = _form.addStringInput(Message.requestSearchTerm());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    _form.parse();

    for (Work w : _receiver.performSearch(_searchKey.value())) {
      _display.addLine(w.getDescription());
    }
    //_display.addLine(_receiver.performSearch(_searchKey.value()));
    _display.display();
  }
  
}
