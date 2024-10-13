#include <iostream>
#include <string>
#include <queue>
using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    bool isIssued;

    Book(int bookID, string bookTitle, string bookAuthor)
        : id(bookID), title(bookTitle), author(bookAuthor), isIssued(false) {}

    void display() const {
        cout << "ID: " << id << "\nTitle: " << title << "\nAuthor: " << author 
             << "\nStatus: " << (isIssued ? "Issued" : "Available") << endl;
    }
};

class Node {
public:
    Book book;
    Node* next;

    Node(Book b) : book(b), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    void addBook(const Book& book) {
        Node* newNode = new Node(book);
        newNode->next = head;
        head = newNode;
    }

    void deleteBook(int id) {
        Node* current = head;
        Node* previous = nullptr;
        while (current != nullptr && current->book.id != id) {
            previous = current;
            current = current->next;
        }
        if (current == nullptr) {
            cout << "Book not found.\n";
            return;
        }
        if (previous == nullptr) {
            head = head->next;  // Deleting the head node
        } else {
            previous->next = current->next;  // Deleting a node in between or at the end
        }
        delete current;
        cout << "Book deleted successfully.\n";
    }

    Node* searchBookByID(int id) const {
        Node* current = head;
        while (current != nullptr) {
            if (current->book.id == id) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    Node* searchBookByTitle(const string& title) const {
        Node* current = head;
        while (current != nullptr) {
            if (current->book.title == title) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void displayAllBooks() const {
        Node* current = head;
        while (current != nullptr) {
            current->book.display();
            cout << "----------------------\n";
            current = current->next;
        }
    }

    void quickSortBooks(bool sortByID) {
        if (!head || !head->next) return;
        head = quickSort(head, sortByID);
    }

private:
    Node* quickSort(Node* head, bool sortByID) {
        if (!head || !head->next) return head;
        Node* pivot = head;
        Node* current = head->next;
        Node* lessHead = nullptr;
        Node* lessTail = nullptr;
        Node* greaterHead = nullptr;
        Node* greaterTail = nullptr;

        while (current != nullptr) {
            Node* next = current->next;
            current->next = nullptr;

            if ((sortByID && current->book.id < pivot->book.id) ||
                (!sortByID && current->book.title < pivot->book.title)) {
                if (!lessHead) {
                    lessHead = lessTail = current;
                } else {
                    lessTail->next = current;
                    lessTail = current;
                }
            } else {
                if (!greaterHead) {
                    greaterHead = greaterTail = current;
                } else {
                    greaterTail->next = current;
                    greaterTail = current;
                }
            }
            current = next;
        }

        lessHead = quickSort(lessHead, sortByID);
        greaterHead = quickSort(greaterHead, sortByID);
        return concatenate(lessHead, pivot, greaterHead);
    }

    Node* concatenate(Node* lessHead, Node* pivot, Node* greaterHead) {
        Node* result = pivot;
        if (lessHead) {
            result = lessHead;
            Node* temp = lessHead;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = pivot;
        }
        pivot->next = greaterHead;
        return result;
    }
};

class Library {
private:
    LinkedList books;
    queue<int> issuedBooks;

public:
    void addBook(int id, string title, string author) {
        Book book(id, title, author);
        books.addBook(book);
    }

    void deleteBook(int id) {
        books.deleteBook(id);
    }

    void searchBookByID(int id) const {
        Node* result = books.searchBookByID(id);
        if (result) {
            result->book.display();
        } else {
            cout << "Book not found.\n";
        }
    }

    void searchBookByTitle(const string& title) const {
        Node* result = books.searchBookByTitle(title);
        if (result) {
            result->book.display();
        } else {
            cout << "Book not found.\n";
        }
    }

    void issueBook(int id) {
        Node* bookNode = books.searchBookByID(id);
        if (bookNode && !bookNode->book.isIssued) {
            bookNode->book.isIssued = true;
            issuedBooks.push(id);
            cout << "Book issued successfully.\n";
        } else {
            cout << "Book not available for issuing.\n";
        }
    }

    void returnBook(int id) {
        Node* bookNode = books.searchBookByID(id);
        if (bookNode && bookNode->book.isIssued) {
            bookNode->book.isIssued = false;
            if (!issuedBooks.empty() && issuedBooks.front() == id) {
                issuedBooks.pop();
            }
            cout << "Book returned successfully.\n";
        } else {
            cout << "Book not found or not issued.\n";
        }
    }

    void listAllBooks(bool sortByID = true) {
        books.quickSortBooks(sortByID);
        books.displayAllBooks();
    }
};

void displayMenu() {
    cout << "Library Management System\n";
    cout << "1. Add New Book\n";
    cout << "2. Search Book by ID\n";
    cout << "3. Search Book by Title\n";
    cout << "4. Issue Book\n";
    cout << "5. Return Book\n";
    cout << "6. List All Books\n";
    cout << "7. Delete Book\n";
    cout << "8. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    Library library;
    int choice, id;
    string title, author;

    while (true) {
        displayMenu();
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "Enter Book ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter Book Title: ";
            getline(cin, title);
            cout << "Enter Book Author: ";
            getline(cin, author);
            library.addBook(id, title, author);
            break;
        case 2:
            cout << "Enter Book ID: ";
            cin >> id;
            library.searchBookByID(id);
            break;
        case 3:
            cout << "Enter Book Title: ";
            cin.ignore();
            getline(cin, title);
            library.searchBookByTitle(title);
            break;
        case 4:
            cout << "Enter Book ID: ";
            cin >> id;
            library.issueBook(id);
            break;
        case 5:
            cout << "Enter Book ID: ";
            cin >> id;
            library.returnBook(id);
            break;
        case 6:
            cout << "Sort by (1) ID or (2) Title? ";
            int sortChoice;
            cin >> sortChoice;
            library.listAllBooks(sortChoice == 1);
            break;
        case 7:
            cout << "Enter Book ID: ";
            cin >> id;
            library.deleteBook(id);
            break;
        case 8:
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
