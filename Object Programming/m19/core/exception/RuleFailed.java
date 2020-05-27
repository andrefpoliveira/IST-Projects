package m19.core.exception;

public class RuleFailed extends Exception {

  /** Serial number for serialization. */
  static final long serialVersionUID = 201901094040L;

  private int _userID;
  private int _workID;
  private int _ruleID;

  /**
   * @param name
   */
  public RuleFailed(int userID, int workID, int ruleID) {
    _userID = userID;
    _workID = workID;
    _ruleID = ruleID;
  }

  public int getUserID() {
    return _userID;
  }

  public int getWorkID() {
    return _workID;
  }

  public int getRuleID() {
    return _ruleID;
  }

}
