package m19.app.main;

import java.io.IOException;

import m19.core.LibraryManager;
import m19.core.exception.MissingFileAssociationException;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.Command;

/**
 * 4.1.1. Save to file under current name (if unnamed, query for name).
 */
public class DoSave extends Command<LibraryManager> {
  
  private Input<String> _filename;

  /**
   * @param receiver
   */
  public DoSave(LibraryManager receiver) {
    super(Label.SAVE, receiver);
    _filename = _form.addStringInput(Message.newSaveAs());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    try {
      _receiver.save();
    } catch (IOException | MissingFileAssociationException | NullPointerException e) {
      _form.parse();
      try {
        _receiver.saveAs(_filename.value());
      } catch (IOException | MissingFileAssociationException ex) {
        System.err.println("Erro ao guardar o estado //Debug");
      }
    }
  }
}
