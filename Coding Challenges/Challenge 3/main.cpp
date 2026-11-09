#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

class Book {
private:
    string title, author, id;
    int pages;
    bool borrowed;
public:
    Book(string t, string a, int p, string i, bool b)
        : title(t), author(a), id(i), pages(p), borrowed(b) {}

    string getId() const { return id; }
    bool isBorrowed() const { return borrowed; }
    void setBorrowed(bool value) { borrowed = value; }
    void display() const {
        cout << "\nTitle: " << title << "\nAuthor: " << author
             << "\nPages: " << pages << "\nID: " << id
             << "\nStatus: " << (borrowed ? "Borrowed" : "Available") << "\n";
    }
    string toFileLine() const {
        return title + "," + author + "," + to_string(pages) + "," + id
               + "," + (borrowed ? "true" : "false");
    }
};

bool loadBooks(const string& filename, vector<Book>& books) {
    ifstream file(filename);
    if (!file) return false;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        string title, author, pagesText, id, status;
        stringstream row(line);
        if (!getline(row, title, ',') || !getline(row, author, ',') ||
            !getline(row, pagesText, ',') || !getline(row, id, ',') ||
            !getline(row, status)) {
            cerr << "Skipping incomplete book record.\n";
            continue;
        }
        try {
            int pages = stoi(pagesText);
            if (pages < 0 || (status != "true" && status != "false")) {
                cerr << "Skipping invalid book record.\n";
                continue;
            }
            books.emplace_back(title, author, pages, id, status == "true");
        } catch (const exception&) {
            cerr << "Skipping record with invalid page count.\n";
        }
    }
    return true;
}

bool saveBooks(const string& filename, const vector<Book>& books) {
    ofstream file(filename);
    if (!file) return false;
    for (const Book& book : books) file << book.toFileLine() << '\n';
    file.flush();
    return file.good();
}

Book* findBook(vector<Book>& books, const string& id) {
    for (Book& book : books)
        if (book.getId() == id) return &book;
    return nullptr;
}

int main() {
    const string filename = "bookData.txt";
    vector<Book> books;
    if (!loadBooks(filename, books)) {
        cerr << "Could not open bookData.txt. Keep it beside main.exe.\n";
        return 1;
    }
    if (books.empty()) {
        cout << "No books found in bookData.txt.\n";
        return 0;
    }

    string choice;
    do {
        cout << "\n--- Library Book Manager ---\n"
             << "1. View all books\n2. View a book by ID\n"
             << "3. Borrow a book\n4. Return a book\n5. Exit\nChoose: ";
        if (!getline(cin, choice)) break;
        if (choice == "1") {
            for (const Book& book : books) book.display();
        } else if (choice == "2" || choice == "3" || choice == "4") {
            string id;
            cout << "Enter book ID (e.g. 001): ";
            if (!getline(cin, id)) break;
            Book* book = findBook(books, id);
            if (book == nullptr) {
                cout << "Book not found.\n";
            } else if (choice == "2") {
                book->display();
            } else if (choice == "3") {
                if (book->isBorrowed()) cout << "Book is already borrowed.\n";
                else {
                    book->setBorrowed(true);
                    if (saveBooks(filename, books)) cout << "Book borrowed successfully.\n";
                    else { book->setBorrowed(false); cerr << "Could not save changes.\n"; }
                }
            } else {
                if (!book->isBorrowed()) cout << "Book is already available.\n";
                else {
                    book->setBorrowed(false);
                    if (saveBooks(filename, books)) cout << "Book returned successfully.\n";
                    else { book->setBorrowed(true); cerr << "Could not save changes.\n"; }
                }
            }
        } else if (choice == "5") {
            cout << "Goodbye!\n";
        } else {
            cout << "Invalid option. Please enter 1 to 5.\n";
        }
    } while (choice != "5");
    return 0;
}
