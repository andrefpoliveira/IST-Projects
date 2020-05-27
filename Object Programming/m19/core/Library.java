package m19.core;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import m19.core.exception.*;
import pt.tecnico.po.ui.Command;

/**
 * Class that represents the library as a whole.
 */
public class Library implements Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201901101348L;

  // FIXME define attributes
  private Date _date;
  private Parser _parser;

  private Map<Integer, User> _users;
  private Map<Integer, Work> _works;

  private int _nextWorkId;
  private int _nextUserId;

  private List<Rule> _rules;

  private String _filename = "/statefile.ser";

  public Library() {
    _date = new Date();
    _parser = new Parser(this);
    _users = new HashMap<>();
    _works = new HashMap<>();
    _rules = new ArrayList<Rule>();
    _rules.add(new CheckRequestTwice(1));
    _rules.add(new CheckUserSuspended(2));
    _rules.add(new CheckNumCopies(3));
    _rules.add(new CheckNumRequests(4));
    _rules.add(new CheckReferenceWork(5));
    _rules.add(new CheckWorkPrice(6));
    _nextWorkId = _nextUserId = 0;
  }

  /**
   * Returns the next user Id
   */
  public int getNextUserId() {
    return _nextUserId;
  }

  /**
   * Returns the next work Id
   */
  public int getNextWorkId() {
    return _nextWorkId;
  }

  /**
   * Returns the number of users registered
   */
  public int getUsersSize() {
    return _users.size();
  }

  /**
   * Returns the number of works registered
   */
  public int getWorksSize() {
    return _works.size();
  }

  public User getUser(int id){
    return _users.get(id);
  }

  public Work getWork(int id){
    return _works.get(id);
  }

  /**
   * Adds a new User
   *
   * @param u
   *        User to add
   */
  public int addUser(User u) {
    _users.put(_nextUserId, u);
    return _nextUserId++;
  }

  /**
   * Adds a new Work
   *
   * @param w
   *        Work to add
   */
  public int addWork(Work w) {
    _works.put(_nextWorkId, w);
    return _nextWorkId++;
  }

  /**
   * Registers a new user
   *
   * @param name
   *        New user's name
   * @param email
   *        New user's email
   */
  public int registerUser(String name, String email) throws FailedToRegisterUser{
    if (name.isEmpty() || email.isEmpty()) {
      throw new FailedToRegisterUser();
    }
    return addUser(new User(name, email,_nextUserId));
  }

  /**
   * Shows a user by its id
   *
   * @param id
   *        Id of the user
   *
   * @throws InvalidIdNumber
   */
  public User showUser(int id) throws InvalidUserIdNumber{
    if (id < 0 || id >= _users.size())
      throw new InvalidUserIdNumber();
    else
      return _users.get(id);
  }

  /**
    * Shows all users ordered by name
   */
  public List<User> showUsers() {
    List<User> temp = new ArrayList<User>(_users.values());
    temp.sort(Comparator.comparing(User::getLowerName));
    return temp;
  }

  /**
   * Shows a work by its id
   *
   * @param id
   *        Id of the work
   *
   * @throws InvalidIdNumber
   */
  public Work showWork(int id) throws InvalidWorkIdNumber{
    if (id < 0 || id >= _works.size())
      throw new InvalidWorkIdNumber();
    else
      return _works.get(id);
  }

  /**
   * Shows all works ordered by id
   */
  public List<Work> showWorks() {
    List<Work> temp = new ArrayList<Work>(_works.values());
    temp.sort(Comparator.comparing(Work::getId));
    return temp;
  }

  /**
   * Shows all works that contains an expression ordered by id
   *
   * @param searchKey
   *        Expression to search for
   */
  public List<Work> performSearch(String searchKey) {
    List<Work> works = new ArrayList<Work>(_works.values());
    List<Work> temp = new ArrayList<Work>();

    for (Work w : works) {
      if (w.getDescription().toLowerCase().contains(searchKey.toLowerCase())) {
        temp.add(w);
      }
    }

    temp.sort(Comparator.comparing(Work::getId));
    return temp;
  }

  /**
   * Shows the current date
   */
  public int getCurrentDate() {
    return _date.getCurrentDate();
  }

  /**
   * Advances the date
   *
   * @param nDays
   *        Number of days to advance
   */
  public void advanceDay(int nDays) {
    if (nDays > 0)
      _date.advanceDay(nDays);
      checkAllUsersAreOutOfDate();
  }

  public void payFine(int id) throws InvalidUserIdNumber, UserIsActive {
    if (id < 0 || id >= _users.size())
      throw new InvalidUserIdNumber();

    User user = _users.get(id);

    if (user.isActive()) {
      throw new UserIsActive();
    } else {
      user.payFine();
      if (!checkUserIsOutOfDate(id)) {
        user.setActive();
      }
    }
  }

  public int requestWork(int userId, int workId) throws RuleFailed, InvalidUserIdNumber, InvalidWorkIdNumber{
    if (userId < 0 || userId >= _users.size())
      throw new InvalidUserIdNumber();

    if (workId < 0 || workId >= _works.size())
      throw new InvalidWorkIdNumber();

    User user = _users.get(userId);
    Work work = _works.get(workId);
    int deadline;
    for(Rule rule : _rules)
      rule.checkRule(user, work);

    deadline = getCurrentDate() + user.getBehavior().getDeadline(work.getNumCopies());
    user.addRequest(deadline,work);
    work.requestWork();
    return deadline;
  }

  public int returnWork(int userId, int workId) throws MissingFileAssociationException, InvalidUserIdNumber, InvalidWorkIdNumber {
    if (userId < 0 || userId >= _users.size())
      throw new InvalidUserIdNumber();

    if (workId < 0 || workId >= _works.size())
      throw new InvalidWorkIdNumber();

    User user = _users.get(userId);
    Work work = _works.get(workId);

    if (user.hasWork(work)) {
    	int workDeadline = user.removeWork(work);
    	int date = getCurrentDate();
    	int dayDifference = user.getBehavior().checkNumReturn(date,workDeadline,user);
      user.addFine(5*dayDifference);
      checkUserIsOutOfDate(userId);
      work.getReturnNotification().update(work.getDescription());
      return user.getFine();
    } else {
      throw new MissingFileAssociationException();
    }
  }

  public void addFine(int userId, int fine) {
    User user = _users.get(userId);
    user.addFine(fine);
  }

  public void checkAllUsersAreOutOfDate() {
    for (int i = 0; i < _users.size(); i++) {
      checkUserIsOutOfDate(i);
    }
  }

  public boolean checkUserIsOutOfDate(int id) {
    User user = _users.get(id);
    List<Request> userRequests = user.getRequests();

    for (Request rq: userRequests) {
      if (getCurrentDate() > rq.getDeadline()) {
        user.setNotActive();
        return true;
      }
    }
    if (user.getFine() > 0) {
      user.setNotActive();
      return true;
    }

    user.setActive();
    return false;
  }

  public void addUserNotification(int userId, int workId){
    User user = _users.get(userId);
    Work work = _works.get(workId);
    work.getReturnNotification().add(user);
  }

  public void removeUserNotification(int userId){
    User user = _users.get(userId);
    user.removeNotifications();

  }

  /**
   * Read the text input file at the beginning of the program and populates the
   * instances of the various possible types (books, DVDs, users).
   *
   * @param filename
   *          name of the file to load
   * @throws BadEntrySpecificationException
   * @throws IOException
   */
  void importFile(String filename) throws BadEntrySpecificationException, IOException {
    _parser.parseFile(filename);
  }

}
