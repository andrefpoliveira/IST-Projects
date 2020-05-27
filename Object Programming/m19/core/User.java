package m19.core;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class User implements Observer{
    private int _fine;
    private int _id;
    private int _numReturn;
    private boolean _isActive;
    private String _name;
    private String _email;
    private UserBehavior _behavior;
    private ArrayList<Request> _requests;
    private ArrayList<Notification> _notifications;

    private static final long serialVersionUID = 201901101342L;

    public User(String name, String email, int id) {
        _name = name;
        _email = email;
        _id = id;
        _fine = 0;
        _numReturn = 0;
	      _isActive= true;
        _behavior = new BehaviorNormal();
        _requests = new ArrayList<Request>();
        _notifications = new ArrayList<Notification>();
    }

    public String getName() {
        return _name;
    }

    public int getFine() {
      return _fine;
    }

    public int getId() {
        return _id;
    }

    public int getNumReturn(){
        return _numReturn;
    }

    public UserBehavior getBehavior(){
        return _behavior;
    }

    public String getLowerName() {
        return _name.toLowerCase();
    }

    protected boolean isActive() {
        return _isActive;
    }

    public void setActive() {
        _isActive = true;
    }

    public void setNotActive() {
        _isActive = false;
    }

    public void setNumReturnSuccess(){
        if(_numReturn < 0)
            _numReturn = 1;
        else {
            _numReturn ++;
        }
    }

    public void setNumReturnFail(){
        if(_numReturn > 0)
            _numReturn = -1;
        else {
            _numReturn --;
        }
    }

    public void setCumpridor(){
        _behavior = new BehaviorCumpridor();
    }

    public void setNormal(){
        _behavior = new BehaviorNormal();
    }

    public void setFaltoso(){
        _behavior = new BehaviorFaltoso();
    }

    public String getDescription() {
        String active = "";
        if (_isActive)
            active = "ACTIVO";
        else
            active = "SUSPENSO - EUR " + _fine;

        return _id + " - " + _name + " - " + _email + " - " + _behavior.getBehavior() + " - " + active;
    }

    public List<Notification> getNotifications(){
        return _notifications;
    }

    public List<Request> getRequests(){
        return _requests;
    }

    public void addRequest(int deadline, Work work){
        _requests.add(new Request(deadline, work, this));
    }

    public boolean hasWork(Work w) {
        for (int i = 0; i < _requests.size(); i++) {
            if (w.equals(_requests.get(i).getWork())) {
                return true;
            }
        }
        return false;
    }

    public int removeWork(Work w) {
        Iterator it = _requests.iterator();
        while (it.hasNext()) {
            Request rq = (Request)it.next();
            if (rq.getWork().equals(w)) {
                w.returnWork();
                _requests.remove(rq);
                return rq.getDeadline();
            }
        }
        return -1;
    }

    public void addFine(int fine) {
      _fine += fine;
    }

    public void payFine() {
      _fine = 0;
    }

    @Override
    public void update(Notification notification){
        _notifications.add(notification);
    }

    public void removeNotifications(){
        for(Notification notification : _notifications)
            notification.remove(this);
        _notifications.clear();
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof User) {
            User other = (User)o;
            return other.hashCode() == this.hashCode();
        }
        return false;
    }

    @Override
    public int hashCode() {
        return _id;
    }
}
