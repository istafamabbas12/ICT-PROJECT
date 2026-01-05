#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <ctime>

using namespace std;

// Forward declarations
class Book;
class User;
class Transaction;

// Book class
class Book {
private:
    int id;
    string title;
    string author;
    string isbn;
    int publicationYear;
    bool isAvailable;
    
public:
    Book(int id, string title, string author, string isbn, int year) 
        : id(id), title(title), author(author), isbn(isbn), 
          publicationYear(year), isAvailable(true) {}
    
    // Getters
    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getISBN() const { return isbn; }
    int getYear() const { return publicationYear; }
    bool getAvailability() const { return isAvailable; }
    
    // Setters
    void setAvailability(bool status) { isAvailable = status; }
    
    void display() const {
        cout << "ID: " << id << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "ISBN: " << isbn << endl;
        cout << "Year: " << publicationYear << endl;
        cout << "Status: " << (isAvailable ? "Available" : "Borrowed") << endl;
        cout << "-------------------" << endl;
    }
};

// User class
class User {
private:
    int id;
    string name;
    string email;
    string membershipDate;
    int borrowedBooks;
    
public:
    User(int id, string name, string email, string date) 
        : id(id), name(name), email(email), membershipDate(date), borrowedBooks(0) {}
    
    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getMembershipDate() const { return membershipDate; }
    int getBorrowedCount() const { return borrowedBooks; }
    
    // Methods to manage borrowed books
    bool canBorrow() const { return borrowedBooks < 5; } // Max 5 books per user
    void incrementBorrowed() { borrowedBooks++; }
    void decrementBorrowed() { if(borrowedBooks > 0) borrowedBooks--; }
    
    void display() const {
        cout << "User ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Membership Date: " << membershipDate << endl;
        cout << "Books Borrowed: " << borrowedBooks << "/5" << endl;
        cout << "-------------------" << endl;
    }
};

// Transaction class
class Transaction {
private:
    int transactionId;
    int userId;
    int bookId;
    string borrowDate;
    string returnDate;
    bool isReturned;
    
public:
    Transaction(int transId, int uId, int bId, string bDate) 
        : transactionId(transId), userId(uId), bookId(bId), 
          borrowDate(bDate), isReturned(false), returnDate("Not Returned") {}
    
    // Getters
    int getTransactionId() const { return transactionId; }
    int getUserId() const { return userId; }
    int getBookId() const { return bookId; }
    string getBorrowDate() const { return borrowDate; }
    string getReturnDate() const { return returnDate; }  // Fixed: Added getter
    bool getReturnStatus() const { return isReturned; }
    
    // Setters
    void setReturnDate(string date) { returnDate = date; }
    void markReturned() { isReturned = true; }
    
    void display() const {
        cout << "Transaction ID: " << transactionId << endl;
        cout << "User ID: " << userId << endl;
        cout << "Book ID: " << bookId << endl;
        cout << "Borrow Date: " << borrowDate << endl;
        if(isReturned) {
            cout << "Return Date: " << returnDate << endl;
        } else {
            cout << "Status: Not Returned" << endl;
        }
        cout << "-------------------" << endl;
    }
};

// Digital Library System class
class DigitalLibrary {
private:
    vector<Book> books;
    vector<User> users;
    vector<Transaction> transactions;
    int nextBookId;
    int nextUserId;
    int nextTransactionId;
    
    string getCurrentDate() {
        time_t now = time(0);
        tm* localTime = localtime(&now);
        string date = to_string(localTime->tm_year + 1900) + "-" +
                      to_string(localTime->tm_mon + 1) + "-" +
                      to_string(localTime->tm_mday);
        return date;
    }
    
    // Helper function to find user by ID
    User* findUserById(int userId) {
        for(auto& user : users) {
            if(user.getId() == userId) {
                return &user;
            }
        }
        return nullptr;
    }
    
    // Helper function to find book by ID
    Book* findBookById(int bookId) {
        for(auto& book : books) {
            if(book.getId() == bookId) {
                return &book;
            }
        }
        return nullptr;
    }
    
    // Helper function to find transaction by ID
    Transaction* findTransactionById(int transId) {
        for(auto& trans : transactions) {
            if(trans.getTransactionId() == transId) {
                return &trans;
            }
        }
        return nullptr;
    }
    
public:
    DigitalLibrary() : nextBookId(1), nextUserId(1), nextTransactionId(1) {
        loadSampleData();
        loadFromFile(); // Load saved data if available
    }
    
    void loadSampleData() {
        // Add some sample books
        books.push_back(Book(nextBookId++, "The Great Gatsby", "F. Scott Fitzgerald", "9780743273565", 1925));
        books.push_back(Book(nextBookId++, "To Kill a Mockingbird", "Harper Lee", "9780446310789", 1960));
        books.push_back(Book(nextBookId++, "1984", "George Orwell", "9780451524935", 1949));
        books.push_back(Book(nextBookId++, "Pride and Prejudice", "Jane Austen", "9780141439518", 1813));
        books.push_back(Book(nextBookId++, "The Hobbit", "J.R.R. Tolkien", "9780547928227", 1937));
        
        // Add some sample users
        users.push_back(User(nextUserId++, "John Doe", "john@email.com", getCurrentDate()));
        users.push_back(User(nextUserId++, "Jane Smith", "jane@email.com", "2023-01-15"));
        users.push_back(User(nextUserId++, "Bob Johnson", "bob@email.com", "2023-03-20"));
    }
    
    // Book Management
    void addBook(string title, string author, string isbn, int year) {
        books.push_back(Book(nextBookId, title, author, isbn, year));
        cout << "Book added successfully! Book ID: " << nextBookId << endl;
        nextBookId++;
        saveToFile(); // Auto-save for CLI
    }

    void addBookPrompt() {
        string title, author, isbn;
        int year;
        
        cout << "\n=== Add New Book ===" << endl;
        cout << "Enter book title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter author: ";
        getline(cin, author);
        cout << "Enter ISBN: ";
        getline(cin, isbn);
        cout << "Enter publication year: ";
        cin >> year;
        
        addBook(title, author, isbn, year);
    }
    
    void searchBook() {
        int choice;
        cout << "\n=== Search Book ===" << endl;
        cout << "1. Search by Title" << endl;
        cout << "2. Search by Author" << endl;
        cout << "3. Search by ISBN" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        
        if(choice < 1 || choice > 3) {
            cout << "Invalid choice!" << endl;
            return;
        }
        
        string query;
        cout << "Enter search query: ";
        cin.ignore();
        getline(cin, query);
        
        bool found = false;
        for(const auto& book : books) {
            bool match = false;
            
            switch(choice) {
                case 1:
                    // Case-insensitive search
                    {
                        string title = book.getTitle();
                        transform(title.begin(), title.end(), title.begin(), ::tolower);
                        string searchQuery = query;
                        transform(searchQuery.begin(), searchQuery.end(), searchQuery.begin(), ::tolower);
                        if(title.find(searchQuery) != string::npos) match = true;
                    }
                    break;
                case 2:
                    {
                        string author = book.getAuthor();
                        transform(author.begin(), author.end(), author.begin(), ::tolower);
                        string searchQuery = query;
                        transform(searchQuery.begin(), searchQuery.end(), searchQuery.begin(), ::tolower);
                        if(author.find(searchQuery) != string::npos) match = true;
                    }
                    break;
                case 3:
                    if(book.getISBN().find(query) != string::npos) match = true;
                    break;
            }
            
            if(match) {
                book.display();
                found = true;
            }
        }
        
        if(!found) {
            cout << "No books found!" << endl;
        }
    }
    
    void displayAllBooks() {
        cout << "\n=== All Books (" << books.size() << ") ===" << endl;
        if(books.empty()) {
            cout << "No books in library!" << endl;
            return;
        }
        
        for(const auto& book : books) {
            book.display();
        }
    }
    
    // User Management
    void addUser() {
        string name, email;
        
        cout << "\n=== Add New User ===" << endl;
        cout << "Enter name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter email: ";
        getline(cin, email);
        
        users.push_back(User(nextUserId, name, email, getCurrentDate()));
        cout << "User added successfully! User ID: " << nextUserId << endl;
        nextUserId++;
    }
    
    void displayAllUsers() {
        cout << "\n=== All Users (" << users.size() << ") ===" << endl;
        if(users.empty()) {
            cout << "No users registered!" << endl;
            return;
        }
        
        for(const auto& user : users) {
            user.display();
        }
    }
    
    // Transaction Management
    void borrowBook(int userId, int bookId) {
        User* userPtr = findUserById(userId);
        Book* bookPtr = findBookById(bookId);
        
        if(userPtr == nullptr) {
            cout << "Error: User not found!" << endl;
            return;
        }
        
        if(bookPtr == nullptr) {
            cout << "Error: Book not found!" << endl;
            return;
        }
        
        if(!userPtr->canBorrow()) {
            cout << "Error: User has reached maximum borrowing limit (5 books)!" << endl;
            return;
        }
        
        if(!bookPtr->getAvailability()) {
            cout << "Error: Book is not available!" << endl;
            return;
        }
        
        // Update book status
        bookPtr->setAvailability(false);
        
        // Update user's borrowed count
        userPtr->incrementBorrowed();
        
        // Create transaction
        transactions.push_back(Transaction(nextTransactionId, userId, bookId, getCurrentDate()));
        
        cout << "Book borrowed successfully!" << endl;
        cout << "Transaction ID: " << nextTransactionId << endl;
        nextTransactionId++;
        saveToFile(); // Auto-save for CLI
    }

    void borrowBookPrompt() {
        int userId, bookId;
        
        cout << "\n=== Borrow Book ===" << endl;
        cout << "Enter User ID: ";
        cin >> userId;
        cout << "Enter Book ID: ";
        cin >> bookId;
        
        borrowBook(userId, bookId);
    }
    
    void returnBook() {
        int transactionId;
        
        cout << "\n=== Return Book ===" << endl;
        cout << "Enter Transaction ID: ";
        cin >> transactionId;
        
        Transaction* transPtr = findTransactionById(transactionId);
        
        if(transPtr == nullptr) {
            cout << "Transaction not found!" << endl;
            return;
        }
        
        if(transPtr->getReturnStatus()) {
            cout << "Book already returned!" << endl;
            return;
        }
        
        // Find and update book
        Book* bookPtr = findBookById(transPtr->getBookId());
        if(bookPtr != nullptr) {
            bookPtr->setAvailability(true);
        }
        
        // Find and update user
        User* userPtr = findUserById(transPtr->getUserId());
        if(userPtr != nullptr) {
            userPtr->decrementBorrowed();
        }
        
        // Update transaction
        transPtr->setReturnDate(getCurrentDate());
        transPtr->markReturned();
        
        cout << "Book returned successfully!" << endl;
    }
    
    void displayAllTransactions() {
        cout << "\n=== All Transactions (" << transactions.size() << ") ===" << endl;
        if(transactions.empty()) {
            cout << "No transactions!" << endl;
            return;
        }
        
        for(const auto& transaction : transactions) {
            transaction.display();
        }
    }
    
    void displayBorrowedBooks() {
        cout << "\n=== Currently Borrowed Books ===" << endl;
        bool found = false;
        
        for(const auto& book : books) {
            if(!book.getAvailability()) {
                book.display();
                found = true;
            }
        }
        
        if(!found) {
            cout << "No books are currently borrowed." << endl;
        }
    }
    
    void saveToFile() {
        ofstream bookFile("books.txt");
        ofstream userFile("users.txt");
        ofstream transFile("transactions.txt");
        
        if(!bookFile.is_open() || !userFile.is_open() || !transFile.is_open()) {
            cout << "Error opening files for writing!" << endl;
            return;
        }
        
        // Save books
        for(const auto& book : books) {
            bookFile << book.getId() << "," << book.getTitle() << "," 
                    << book.getAuthor() << "," << book.getISBN() << "," 
                    << book.getYear() << "," << book.getAvailability() << endl;
        }
        
        // Save users
        for(const auto& user : users) {
            userFile << user.getId() << "," << user.getName() << "," 
                    << user.getEmail() << "," << user.getMembershipDate() << "," 
                    << user.getBorrowedCount() << endl;
        }
        
        // Save transactions
        for(const auto& trans : transactions) {
            transFile << trans.getTransactionId() << "," << trans.getUserId() << "," 
                     << trans.getBookId() << "," << trans.getBorrowDate() << "," 
                     << trans.getReturnDate() << "," << trans.getReturnStatus() << endl;
        }
        
        bookFile.close();
        userFile.close();
        transFile.close();
        
        cout << "Data saved to files (books.txt, users.txt, transactions.txt)!" << endl;
    }
    
    void loadFromFile() {
        ifstream bookFile("books.txt");
        ifstream userFile("users.txt");
        ifstream transFile("transactions.txt");
        
        if(!bookFile.is_open()) {
            cout << "No saved data found. Starting with empty library." << endl;
            return;
        }
        
        // Clear existing data
        books.clear();
        users.clear();
        transactions.clear();
        
        // Load books
        string line;
        while(getline(bookFile, line)) {
            vector<string> tokens;
            size_t pos = 0;
            string delimiter = ",";
            
            while((pos = line.find(delimiter)) != string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + delimiter.length());
            }
            tokens.push_back(line);
            
            if(tokens.size() == 6) {
                int id = stoi(tokens[0]);
                bool available = (tokens[5] == "1");
                Book book(id, tokens[1], tokens[2], tokens[3], stoi(tokens[4]));
                book.setAvailability(available);
                books.push_back(book);
                if(id >= nextBookId) nextBookId = id + 1;
            }
        }
        
        // Load users
        while(getline(userFile, line)) {
            vector<string> tokens;
            size_t pos = 0;
            string delimiter = ",";
            
            while((pos = line.find(delimiter)) != string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + delimiter.length());
            }
            tokens.push_back(line);
            
            if(tokens.size() == 5) {
                int id = stoi(tokens[0]);
                User user(id, tokens[1], tokens[2], tokens[3]);
                // Set borrowed count
                for(int i = 0; i < stoi(tokens[4]); i++) {
                    user.incrementBorrowed();
                }
                users.push_back(user);
                if(id >= nextUserId) nextUserId = id + 1;
            }
        }
        
        // Load transactions
        while(getline(transFile, line)) {
            vector<string> tokens;
            size_t pos = 0;
            string delimiter = ",";
            
            while((pos = line.find(delimiter)) != string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + delimiter.length());
            }
            tokens.push_back(line);
            
            if(tokens.size() == 6) {
                int id = stoi(tokens[0]);
                Transaction trans(id, stoi(tokens[1]), stoi(tokens[2]), tokens[3]);
                trans.setReturnDate(tokens[4]);
                if(tokens[5] == "1") {
                    trans.markReturned();
                }
                transactions.push_back(trans);
                if(id >= nextTransactionId) nextTransactionId = id + 1;
            }
        }
        
        bookFile.close();
        userFile.close();
        transFile.close();
        
        cout << "Data loaded successfully!" << endl;
        cout << "Loaded " << books.size() << " books, " 
             << users.size() << " users, and " 
             << transactions.size() << " transactions." << endl;
    }
    
    void displayStatistics() {
        cout << "\n=== Library Statistics ===" << endl;
        cout << "Total Books: " << books.size() << endl;
        
        int availableBooks = 0;
        int borrowedBooks = 0;
        for(const auto& book : books) {
            if(book.getAvailability()) {
                availableBooks++;
            } else {
                borrowedBooks++;
            }
        }
        
        cout << "Available Books: " << availableBooks << endl;
        cout << "Borrowed Books: " << borrowedBooks << endl;
        cout << "Total Users: " << users.size() << endl;
        cout << "Total Transactions: " << transactions.size() << endl;
    }
    
    void displayMenu() {
        while(true) {
            cout << "\n=== DIGITAL LIBRARY MANAGEMENT SYSTEM ===" << endl;
            cout << "1. Add New Book" << endl;
            cout << "2. Search Book" << endl;
            cout << "3. Display All Books" << endl;
            cout << "4. Add New User" << endl;
            cout << "5. Display All Users" << endl;
            cout << "6. Borrow Book" << endl;
            cout << "7. Return Book" << endl;
            cout << "8. Display All Transactions" << endl;
            cout << "9. Display Borrowed Books" << endl;
            cout << "10. Display Statistics" << endl;
            cout << "11. Save Data to Files" << endl;
            cout << "12. Load Data from Files" << endl;
            cout << "13. Exit" << endl;
            cout << "Enter your choice (1-13): ";
            
            int choice;
            cin >> choice;
            
            switch(choice) {
                case 1: addBookPrompt(); break;
                case 2: searchBook(); break;
                case 3: displayAllBooks(); break;
                case 4: addUser(); break;
                case 5: displayAllUsers(); break;
                case 6: borrowBookPrompt(); break;
                case 7: returnBook(); break;
                case 8: displayAllTransactions(); break;
                case 9: displayBorrowedBooks(); break;
                case 10: displayStatistics(); break;
                case 11: saveToFile(); break;
                case 12: loadFromFile(); break;
                case 13: 
                    cout << "\nThank you for using Digital Library System!" << endl;
                    cout << "Goodbye!" << endl;
                    return;
                default:
                    cout << "Invalid choice! Please enter a number between 1 and 13." << endl;
            }
        }
    }


    // New CLI logic
    void handleCLI(int argc, char* argv[]) {
        string command = argv[1];

        if (command == "add_book" && argc >= 6) {
            // usage: add_book <title> <author> <isbn> <year>
            string title = argv[2];
            string author = argv[3];
            string isbn = argv[4];
            int year = stoi(argv[5]);
            addBook(title, author, isbn, year);
        } else if (command == "borrow_book" && argc >= 4) {
             // usage: borrow_book <user_name> <book_id>
             // Simplified: We need to find a user by name or create one for the web
             string userName = argv[2];
             int bookId = stoi(argv[3]);
             
             // Find user ID (simple loop)
             int userId = -1;
             for(const auto& u : users) {
                 if(u.getName() == userName) {
                     userId = u.getId();
                     break;
                 }
             }
             
             if(userId == -1) {
                 // Create temp user for web interface if not found
                 users.push_back(User(nextUserId, userName, userName + "@web.com", getCurrentDate()));
                 userId = nextUserId;
                 nextUserId++;
                 saveToFile();
             }
             
             borrowBook(userId, bookId);

        } else if (command == "return_book" && argc >= 3) {
             // usage: return_book <transaction_id>
             int transactionId = stoi(argv[2]);
             
             Transaction* transPtr = findTransactionById(transactionId);
             
             if(transPtr == nullptr) {
                 cout << "Error: Transaction not found!" << endl;
                 return;
             }
             
             if(transPtr->getReturnStatus()) {
                 cout << "Error: Book already returned!" << endl;
                 return;
             }
             
             // Find and update book
             Book* bookPtr = findBookById(transPtr->getBookId());
             if(bookPtr != nullptr) {
                 bookPtr->setAvailability(true);
             }
             
             // Find and update user
             User* userPtr = findUserById(transPtr->getUserId());
             if(userPtr != nullptr) {
                 userPtr->decrementBorrowed();
             }
             
             // Update transaction
             transPtr->setReturnDate(getCurrentDate());
             transPtr->markReturned();
             
             saveToFile();
             cout << "Book returned successfully!" << endl;

        } else if (command == "get_transactions") {
            // Output JSON-like structure to stdout
            cout << "[";
            for(size_t i = 0; i < transactions.size(); ++i) {
                const auto& t = transactions[i];
                cout << "{\"id\":" << t.getTransactionId() 
                     << ",\"userId\":" << t.getUserId()
                     << ",\"bookId\":" << t.getBookId()
                     << ",\"borrowDate\":\"" << t.getBorrowDate() << "\""
                     << ",\"returnDate\":\"" << t.getReturnDate() << "\""
                     << ",\"isReturned\":" << (t.getReturnStatus() ? "true" : "false") 
                     << "}";
                if(i < transactions.size() - 1) cout << ",";
            }
            cout << "]" << endl;

        } else if (command == "get_books") {
            // Output JSON-like structure to stdout
            cout << "[";
            for(size_t i = 0; i < books.size(); ++i) {
                const auto& b = books[i];
                cout << "{\"id\":" << b.getId() 
                     << ",\"title\":\"" << b.getTitle() << "\""
                     << ",\"author\":\"" << b.getAuthor() << "\""
                     << ",\"isbn\":\"" << b.getISBN() << "\""
                     << ",\"year\":" << b.getYear() 
                     << ",\"available\":" << (b.getAvailability() ? "true" : "false") 
                     << "}";
                if(i < books.size() - 1) cout << ",";
            }
            cout << "]" << endl;
        } else {
            cout << "Unknown command or invalid arguments" << endl;
        }
    }
};

// Main function
int main(int argc, char* argv[]) {
    DigitalLibrary library;
    
    if (argc > 1) {
        library.handleCLI(argc, argv);
        return 0;
    }

    cout << "========================================" << endl;
    cout << "  DIGITAL LIBRARY MANAGEMENT SYSTEM" << endl;
    cout << "========================================" << endl;
    
    library.displayMenu();
    
    return 0;
}