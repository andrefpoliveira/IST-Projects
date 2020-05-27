package m19.core;
import java.io.Serializable;

public abstract class Work implements Serializable{
    private int _id;
    private int _price;
    private int _numCopies;
    private int _numCopiesAvailable;
    private String _title;
    private Category _category;
    private Subject _returnNotification;
    private Subject _requestNotification;

    private static final long serialVersionUID = 201901101343L;

    public Work(int id, int price, int numCopies, String title, Category category) {
        _id = id;
        _price = price;
        _numCopies = _numCopiesAvailable = numCopies;
        _title = title;
        _category = category;
        _returnNotification = new ReturnNotification();
        _requestNotification = new RequestNotification();
    }

    public int getId() {
        return _id;
    }

    public int getPrice() {
        return _price;
    }

    public Category getCategory() {
        return _category;
    }

    public int getNumCopiesAvailable(){
        return _numCopiesAvailable;
    }

    public int getNumCopies() {
        return _numCopies;
    }

    public Subject getReturnNotification(){
        return _returnNotification;
    }

    public Subject getRequesttNotification(){
        return _requestNotification;
    }

    public void requestWork(){
        _numCopiesAvailable--;
    }

    public void returnWork() {
        _numCopiesAvailable++;
    }

    public abstract String getDescription();

    protected String privateDescription(String type) {
        return _id + " - " + _numCopiesAvailable + " de " + _numCopies + " - " + type + " - " + _title + " - " + _price + " - " + _category;
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof Work) {
            Work other = (Work)o;
            return other.hashCode() == this.hashCode();
        }
        return false;
    }

    @Override
    public int hashCode() {
        return _id;
    }
}
