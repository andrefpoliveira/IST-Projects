package m19.core;

public class Book extends Work{
    private String _author;
    private String _isbn;

    public Book(String title, String author, int price, Category category, String isbn, int numberCopies, int id) {
        super(id, price, numberCopies, title, category);
        _author = author;
        _isbn = isbn;
    }

    public String getDescription() {
        return super.privateDescription("Livro") + " - " + _author + " - " + _isbn;
    }
}