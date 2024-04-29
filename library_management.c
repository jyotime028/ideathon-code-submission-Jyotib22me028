#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book {
    int id;
    char name[100];
    char author[100];
    int quantity;
    char date_added[11];
    struct Book *next;
} Book;

typedef struct IssuedBook {
    char name[100];
    int roll_number;
    int year;
    char branch[50];
    int book_id;
    char book_name[100];
    char book_author[100];
    int book_quantity;
    char issue_date[11];
    struct IssuedBook *next;
} IssuedBook;

typedef struct Student {
    char name[100];
    int roll_number;
    char password[20];
    struct Student *next;
} Student;

Book *library = NULL;
IssuedBook *issued_books = NULL;
Student *students = NULL;

// Librarian credentials
char librarian_id[20] = "admin";
char librarian_password[20] = "admin123";

// Function prototypes
void addBook();
void removeBook();
void displayBooks();
void searchBook();
void updateBookDetails();
void issueBook();
void returnBook();
void historyOfIssuedBooks();
void librarianLogin();
void changeLibrarianCredentials();
void studentLogin();
void studentSignUp();
void mainMenu();

#define HASH_TABLE_SIZE 1009  

Book *hashTable[HASH_TABLE_SIZE] = {NULL};  

int hashFunction(int key) {
    return key % HASH_TABLE_SIZE;
}

void addToHashTable(Book *book) {
    int index = hashFunction(book->id);
    book->next = hashTable[index];
    hashTable[index] = book;
}

Book *findBookById(int id) {
    int index = hashFunction(id);
    Book *current = hashTable[index];
    while (current != NULL) {
        if (current->id == id) {
            return current;  // Book found
        }
        current = current->next;
    }
    return NULL;  // Book not found
}

Book *createBook(int id, char *name, char *author, int quantity, char *date_added) {
    Book *newBook = (Book *)malloc(sizeof(Book));
    if (!newBook) return NULL;
    newBook->id = id;
    strcpy(newBook->name, name);
    strcpy(newBook->author, author);
    newBook->quantity = quantity;
    strcpy(newBook->date_added, date_added);
    newBook->next = NULL;
    addToHashTable(newBook);
    return newBook;
}

void addBook() {
    printf("\n** Adding a new book **\n");
    Book *b = (Book *)malloc(sizeof(Book));
    if (!b) {
        printf("Failed to allocate memory.\n");
        return;
    }
    printf("\n");
    printf("Enter Book ID: ");
    scanf("%d", &b->id);
    printf("Enter Book Name: ");
    scanf(" %[^\n]s", b->name);
    printf("Enter Author Name: ");
    scanf(" %[^\n]s", b->author);
    printf("Enter Quantity: ");
    scanf("%d", &b->quantity);
    printf("Enter Adding Date (YYYY-MM-DD): ");
    scanf("%s", b->date_added);
    b->next = library;
    library = b;
    printf("\n");
    addToHashTable(b);
    printf("Book added successfully!\n");
}

void removeBook() {
    int id;
    printf("\n** Remove a Book **\n");
    printf("\n");
    printf("Enter Book ID to remove: ");
    scanf("%d", &id);

    // Remove from linked list
    Book **ptr = &library;
    while (*ptr) {
        Book *current = *ptr;
        if (current->id == id) {
            *ptr = current->next;
            free(current);
            printf("\n");
            printf("Book removed successfully\n");
            break;
        }
        ptr = &current->next;
    }

    // Remove from hash map
    int index = hashFunction(id);
    Book **hashPtr = &hashTable[index];
    while (*hashPtr) {
        Book *current = *hashPtr;
        if (current->id == id) {
            *hashPtr = current->next;
            free(current);
            break;
        }
        hashPtr = &current->next;
    }

    if (!(*ptr) || !(*hashPtr)) {
        printf("\n");
        printf("Book not found!\n");
    }
}

void displayBooks() {
    printf("\n** Displaying all books **\n");
    Book *ptr = library;
    printf("\n");
    if (!ptr) printf("No books in the library.\n");
    while (ptr) {
        printf("ID: %d, Name: %s, Author: %s, Quantity: %d, Added: %s\n",
               ptr->id, ptr->name, ptr->author, ptr->quantity, ptr->date_added);
        ptr = ptr->next;
    }
}


void searchBook() {
    int id;
    printf("\n** Search for a Book **\n");
    printf("\n");
    printf("Enter book ID to search: ");
    scanf("%d", &id);

    Book *foundBook = findBookById(id);

    if (foundBook != NULL) {
        printf("Book ID %d found: %s by %s\n", foundBook->id, foundBook->name, foundBook->author);
    } else {
        printf("Book ID %d not found.\n", id);
    }
}


void updateBookDetails() {
    int id;
    printf("\n** Update Book Details **\n");
    printf("\n");
    printf("Enter Book ID to update: ");
    scanf("%d", &id);

    // Search for the book in the linked list
    Book *ptr = library;
    while (ptr) {
        if (ptr->id == id) {
            printf("Current Name: %s, Enter new Name: ", ptr->name);
            scanf(" %[^\n]s", ptr->name);
            printf("Current Author: %s, Enter new Author: ", ptr->author);
            scanf(" %[^\n]s", ptr->author);
            printf("Current Quantity: %d, Enter new Quantity: ", ptr->quantity);
            scanf("%d", &ptr->quantity);
            printf("\n");
            printf("Book details updated successfully in the linked list!\n");

            // Now update the book details in the hash table
            int index = hashFunction(id);
            Book *hashPtr = hashTable[index];
            while (hashPtr) {
                if (hashPtr->id == id) {
                    strcpy(hashPtr->name, ptr->name);
                    strcpy(hashPtr->author, ptr->author);
                    hashPtr->quantity = ptr->quantity;
                    break;
                }
                hashPtr = hashPtr->next;
            }

            return;
        }
        ptr = ptr->next;
    }

    printf("\n");
    printf("Book not found!\n");
}


IssuedBook *createIssuedBook(char *name, int roll_number, int year, char *branch, int book_id,
                             char *book_name, char *book_author, int book_quantity, char *issue_date) {
    IssuedBook *newIssuedBook = (IssuedBook *)malloc(sizeof(IssuedBook));
    if (!newIssuedBook) return NULL;
    strcpy(newIssuedBook->name, name);
    newIssuedBook->roll_number = roll_number;
    newIssuedBook->year = year;
    strcpy(newIssuedBook->branch, branch);
    newIssuedBook->book_id = book_id;
    strcpy(newIssuedBook->book_name, book_name);
    strcpy(newIssuedBook->book_author, book_author);
    newIssuedBook->book_quantity = book_quantity;
    strcpy(newIssuedBook->issue_date, issue_date);
    newIssuedBook->next = NULL;
    return newIssuedBook;
}

void issueBook() {
    int book_id;
    printf("\n** Issue a Book **\n");
    printf("\n");
    printf("Enter Book ID to issue: ");
    scanf("%d", &book_id);
    Book *book = library;
    while (book) {
        if (book->id == book_id && book->quantity > 0) {
            
            book->quantity--;

            IssuedBook *newIssued = (IssuedBook *)malloc(sizeof(IssuedBook));
            if (!newIssued) {
                printf("Memory allocation failed.\n");
                return;
            }
            printf("Enter your name: ");
            scanf(" %[^\n]s", newIssued->name);
            printf("Enter your roll number: ");
            scanf("%d", &newIssued->roll_number);
            printf("Enter your year of studying: ");
            scanf("%d", &newIssued->year);
            printf("Enter your branch: ");
            scanf(" %[^\n]s", newIssued->branch);
            strcpy(newIssued->book_name, book->name);
            strcpy(newIssued->book_author, book->author);
            newIssued->book_id = book->id;
            newIssued->book_quantity = 1;
            printf("Enter issue date (YYYY-MM-DD): ");
            scanf("%s", newIssued->issue_date);
            newIssued->next = issued_books;
            issued_books = newIssued;

            printf("\n");
            printf("Book issued successfully!\n");

            
            return;
        }
        book = book->next;
    }

    printf("\n");
    printf("Book not available for issue or does not exist!\n");
}


void returnBook() {
    int roll_number, book_id;
    printf("\n** Return a Book **\n");
    printf("\n");
    printf("Enter your roll number: ");
    scanf("%d", &roll_number);
    printf("Enter Book ID to return: ");
    scanf("%d", &book_id);
    IssuedBook **ptr = &issued_books;
    while (*ptr) {
        IssuedBook *current = *ptr;
        if (current->roll_number == roll_number && current->book_id == book_id) {
            // Increment book quantity back in library
            Book *book = library;
            while (book) {
                if (book->id == book_id) {
                    book->quantity++;
                    break;
                }
                book = book->next;
            }
            *ptr = current->next;
            free(current);
            printf("\n");
            printf("Book returned successfully in the library!\n");

            return;
        }
        ptr = &current->next;
    }
    printf("\n");
    printf("No record found for your issued book!\n");
}


void historyOfIssuedBooks() {
    int roll_number;
    printf("\n** History of Issued Books **\n");
    printf("\n");
    printf("Enter your roll number to see history of issued books: ");
    scanf("%d", &roll_number);
    IssuedBook *ptr = issued_books;
    int found = 0;
    while (ptr) {
        if (ptr->roll_number == roll_number) {
            printf("Book ID: %d, Book Name: %s, Author: %s, Issue Date: %s\n",
                   ptr->book_id, ptr->book_name, ptr->book_author, ptr->issue_date);
            found = 1;
        }
        ptr = ptr->next;
    }
    if (!found) {
        printf("\n");
        printf("No issued books found for this roll number!\n");
    }
}

void librarianLogin() {
    char id[20], password[20];
    printf("\n** Librarian Login **\n");
    printf("\n");
    printf("Enter librarian ID: ");
    scanf("%s", id);
    printf("Enter password: ");
    scanf("%s", password);

    if (strcmp(id, librarian_id) == 0 && strcmp(password, librarian_password) == 0) {
        int choice;
        do {
            printf("\n** Librarian Menu **\n");
            printf("\n");
            printf("1. Add Book\n");
            printf("2. Remove Book\n");
            printf("3. Display Books\n");
            printf("4. Search for a Book\n");
            printf("5. Update Book Details\n");
            printf("6. Change Login Details\n");
            printf("7. Return to Main Menu\n");
            printf("\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1: addBook(); break;
                case 2: removeBook(); break;
                case 3: displayBooks(); break;
                case 4: searchBook(); break;
                case 5: updateBookDetails(); break;
                case 6: changeLibrarianCredentials(); break;
                case 7: return;  // Return to main menu
                default: printf("Invalid choice. Please enter a valid option.\n");
            }
        } while (choice != 7);
    } else {
        printf("\n");
        printf("Invalid ID or Password!\n");
    }
}

void changeLibrarianCredentials() {
    char currentPassword[20], newPassword[20];
    printf("\n** Change Librarian Credentials **\n");
    printf("\n");
    printf("Enter current password: ");
    scanf("%s", currentPassword);
    if (strcmp(currentPassword, librarian_password) == 0) {
        printf("Enter new password: ");
        scanf("%s", newPassword);
        strcpy(librarian_password, newPassword);
        printf("\n");
        printf("Password changed successfully!\n");
    } else {
        printf("\n");
        printf("Incorrect password!\n");
    }
}

void studentLogin() {
    int roll_number;
    char password[20];
    printf("\n** Student Login **\n");
    printf("\n");
    printf("Enter your roll number: ");
    scanf("%d", &roll_number);
    printf("Enter your password: ");
    scanf("%s", password);

    Student *ptr = students;
    while (ptr) {
        if (ptr->roll_number == roll_number && strcmp(ptr->password, password) == 0) {
            int choice;
            do {
                printf("\n** Student Menu **\n");
                printf("\n");
                printf("1. Issue Book\n");
                printf("2. Return Book\n");
                printf("3. Display Books\n");
                printf("4. History of Issued Books\n");
                printf("5. Return to Main Menu\n");
                printf("\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                switch (choice) {
                    case 1: issueBook(); break;
                    case 2: returnBook(); break;
                    case 3: displayBooks(); break;
                    case 4: historyOfIssuedBooks(); break;
                    case 5: return;  // Return to main menu
                    default: printf("Invalid choice. Please enter a valid option.\n");
                }
            } while (choice != 5);
            return;
        }
        ptr = ptr->next;
    }
    printf("\n");
    printf("Invalid roll number or password!\n");
}

void studentSignUp() {
    printf("\n** Student Sign Up **\n");
    Student *newStudent = (Student *)malloc(sizeof(Student));
    if (!newStudent) {
        printf("Failed to allocate memory.\n");
        return;
    }
    printf("\n");
    printf("Enter your name: ");
    scanf(" %[^\n]s", newStudent->name);
    printf("Enter your roll number: ");
    scanf("%d", &newStudent->roll_number);
    printf("Create a password: ");
    scanf("%s", newStudent->password);
    newStudent->next = students;
    students = newStudent;
    printf("\n");
    printf("Registration successful! You can now log in using your roll number and password.\n");
}

void addBookToList(Book *book) {
    if (!library) {
        library = book;
    } else {
        Book *current = library;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = book;
    }
}

void readCSV(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file for reading.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int id, quantity;
        char name[100], author[100], date_added[11];

        // Parse the line and extract data
        if (sscanf(line, "%d,%99[^,],%99[^,],%d,%10s", &id, name, author, &quantity, date_added) == 5) {
            Book *newBook = createBook(id, name, author, quantity, date_added);
            if (newBook) {
                addBookToList(newBook);
            } else {
                printf("Failed to create book node.\n");
            }
        } else {
            printf("Error parsing line: %s\n", line);
        }
    }

    fclose(file);
}

void mainMenu() {
    int choice;
    do {
        printf("\n\n");
        printf("\t\t\t     ****************************************\n");
        printf("\t\t\t     *                                      *\n");
        printf("\t\t\t     *                                      *\n");
        printf("\t\t\t     *     ----------------------------     *\n");
        printf("\t\t\t     *         WELCOME TO e-LIBRARY         *\n");
        printf("\t\t\t     *     ----------------------------     *\n");
        printf("\t\t\t     *             IIT JODHPUR              *\n");
        printf("\t\t\t     *                                      *\n");
        printf("\t\t\t     *                                      *\n");
        printf("\t\t\t     ****************************************\n");
        printf("\n\n");
        printf("\n\t\t\t*************************************************\n");
        printf("\n\t\t\t\t      MAIN MENU: \n");
        printf("\n\t\t\t\t     1.LIBRARIAN LOGIN ");
        printf("\n\t\t\t\t     2.STUDENT LOGIN ");
        printf("\n\t\t\t\t     3.STUDENT SIGN UP ");
        printf("\n\t\t\t\t     4.EXIT\n ");
        printf("\n\t\t\t*************************************************\n");
        printf("\n\t\t\t\t      Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: librarianLogin(); break;
            case 2: studentLogin(); break;
            case 3: studentSignUp(); break;
            case 4: printf("Exiting the system.\n"); return;
            default: printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 4);
}

int main() {
    const char *filename = "books.csv";//path of books.csv
    readCSV(filename);
    mainMenu();
    return 0;
}
