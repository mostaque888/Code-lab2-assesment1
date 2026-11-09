#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

struct Book {
    string title, author, id;
    int pages;
    bool borrowed;
};

// Load books from file into vector
void loadBooks(vector<Book>& books, const string& file) {
    ifstream in(file);
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Book b;
        string pagesStr, status;
        getline(ss, b.title, ',');
        getline(ss, b.author, ',');
        getline(ss, pagesStr, ',');
        getline(ss, b.id, ',');
        getline(ss, status);
        b.pages = stoi(pagesStr);
        b.borrowed = (status == "true");
        books.push_back(b);
    }
}

// Save all books back to file
void saveBooks(const vector<Book>& books, const string& file) {
    ofstream out(file);
    for (const Book& b : books)
        out << b.title << "," << b.author << "," << b.pages << ","
            << b.id << "," << (b.borrowed ? "true" : "false") << "\n";
}

// Display a single book
void showBook(const Book& b) {
    cout << "\nTitle: " << b.title
         << "\nAuthor: " << b.author
         << "\nPages: " << b.pages
         << "\nID: " << b.id
         << "\nStatus: " << (b.borrowed ? "Borrowed" : "Available") << "\n";
}

// Find a book index by ID, or -1 if not found
int findBook(const vector<Book>& books, const string& id) {
    for (int i = 0; i < (int)books.size(); i++)
        if (books[i].id == id) return i;
    return -1;
}

int main() {
    const string file = "bookData.txt";
    vector<Book> books;
    loadBooks(books, file);

    string choice;
    while (true) {
        cout << "\n--- Library Book Manager ---\n"
             << "1. View all books\n"
             << "2. View a book by ID\n"
             << "3. Borrow a book\n"
             << "4. Return a book\n"
             << "5. Exit\n"
             << "Choose: ";
        getline(cin, choice);

        if (choice == "1") {
            for (const Book& b : books) showBook(b);
        }
        else if (choice == "2" || choice == "3" || choice == "4") {
            cout << "Enter book ID: ";
            string id;
            getline(cin, id);

            int idx = findBook(books, id);
            if (idx == -1) {
                cout << "Book not found.\n";
                continue;
            }

            Book& b = books[idx];

            if (choice == "2") {
                showBook(b);
            }
            else if (choice == "3") {
                if (b.borrowed) cout << "Already borrowed.\n";
                else {
                    b.borrowed = true;
                    saveBooks(books, file);
                    cout << "Book borrowed successfully.\n";
                }
            }
            else {
                if (!b.borrowed) cout << "Already available.\n";
                else {
                    b.borrowed = false;
                    saveBooks(books, file);
                    cout << "Book returned successfully.\n";
                }
            }
        }
        else if (choice == "5") {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid option.\n";
        }
    }
    return 0;
}



