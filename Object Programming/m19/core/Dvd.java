package m19.core;

public class Dvd extends Work{
    private String _director;
    private String _igac;

    public Dvd(String title, String director, int price, Category category, String igac, int numberCopies, int id) {
        super(id, price, numberCopies, title, category);
        _director = director;
        _igac = igac;
    }

    public String getDescription() {
        return super.privateDescription("DVD") + " - " + _director + " - " + _igac;
    }
}