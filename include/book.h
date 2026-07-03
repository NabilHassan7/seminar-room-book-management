#ifndef BOOK_H
#define BOOK_H

void addBook(void);
void removeBook(void);
void updateBook(void);
void searchBooks(void);
void viewAvailableBooks(void);

int bookExistsById(const char *bookId);

#endif