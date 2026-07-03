#include <stdio.h>
#include <string.h>

#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/book.h"

static void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void readLine(char *buffer, int size)
{
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

static void printBookHeader(void)
{
    printf("\n%-12s %-28s %-22s %-18s %-10s %-10s %-22s %-12s\n",
           "ID", "Title", "Author", "Category", "Total", "Available", "Publisher", "Edition");

    printf("--------------------------------------------------------------------------------------------------------------------------------\n");
}

static void printBookRow(Book book)
{
    printf("%-12s %-28s %-22s %-18s %-10d %-10d %-22s %-12s\n",
           book.id,
           book.title,
           book.author,
           book.category,
           book.totalCopies,
           book.availableCopies,
           book.publicationCompany,
           book.edition);
}

int bookExistsById(const char *bookId)
{
    FILE *file = fopen(BOOKS_FILE, "rb");
    Book book;

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&book, sizeof(Book), 1, file) == 1)
    {
        if (strcmp(book.id, bookId) == 0 && book.isActive == STATUS_ACTIVE)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void addBook(void)
{
    FILE *file;
    Book book;

    printf("\nADD BOOK\n");

    clearInputBuffer();

    printf("Book ID: ");
    readLine(book.id, MAX_ID_LEN);

    if (bookExistsById(book.id))
    {
        printf("\nError: Active book with this ID already exists.\n");
        return;
    }

    printf("Title: ");
    readLine(book.title, MAX_TITLE_LEN);

    printf("Author: ");
    readLine(book.author, MAX_AUTHOR_LEN);

    printf("Category: ");
    readLine(book.category, MAX_CATEGORY_LEN);

    printf("Total Copies: ");
    scanf("%d", &book.totalCopies);

    if (book.totalCopies <= 0)
    {
        printf("\nError: Total copies must be greater than zero.\n");
        return;
    }

    book.availableCopies = book.totalCopies;

    clearInputBuffer();

    printf("Publication Company: ");
    readLine(book.publicationCompany, MAX_COMPANY_LEN);

    printf("Edition: ");
    readLine(book.edition, MAX_EDITION_LEN);

    book.isActive = STATUS_ACTIVE;

    file = fopen(BOOKS_FILE, "ab");

    if (file == NULL)
    {
        printf("\nError: Could not open books file.\n");
        return;
    }

    fwrite(&book, sizeof(Book), 1, file);
    fclose(file);

    printf("\nBook added successfully.\n");
}

void removeBook(void)
{
    FILE *file = fopen(BOOKS_FILE, "rb+");
    Book book;
    char bookId[MAX_ID_LEN];
    int found = 0;

    if (file == NULL)
    {
        printf("\nError: Could not open books file.\n");
        return;
    }

    printf("\nREMOVE BOOK\n");
    clearInputBuffer();

    printf("Enter Book ID: ");
    readLine(bookId, MAX_ID_LEN);

    while (fread(&book, sizeof(Book), 1, file) == 1)
    {
        if (strcmp(book.id, bookId) == 0 && book.isActive == STATUS_ACTIVE)
        {
            book.isActive = STATUS_INACTIVE;

            fseek(file, -(long)sizeof(Book), SEEK_CUR);
            fwrite(&book, sizeof(Book), 1, file);

            found = 1;
            break;
        }
    }

    fclose(file);

    if (found)
    {
        printf("\nBook marked as inactive successfully.\n");
    }
    else
    {
        printf("\nBook not found or already inactive.\n");
    }
}

void updateBook(void)
{
    FILE *file = fopen(BOOKS_FILE, "rb+");
    Book book;
    char bookId[MAX_ID_LEN];
    int found = 0;
    int borrowedCopies;
    int newTotalCopies;

    if (file == NULL)
    {
        printf("\nError: Could not open books file.\n");
        return;
    }

    printf("\nUPDATE BOOK\n");
    clearInputBuffer();

    printf("Enter Book ID: ");
    readLine(bookId, MAX_ID_LEN);

    while (fread(&book, sizeof(Book), 1, file) == 1)
    {
        if (strcmp(book.id, bookId) == 0 && book.isActive == STATUS_ACTIVE)
        {
            found = 1;

            printf("\nCurrent Book Details:\n");
            printBookHeader();
            printBookRow(book);

            printf("\nNew Title: ");
            readLine(book.title, MAX_TITLE_LEN);

            printf("New Author: ");
            readLine(book.author, MAX_AUTHOR_LEN);

            printf("New Category: ");
            readLine(book.category, MAX_CATEGORY_LEN);

            borrowedCopies = book.totalCopies - book.availableCopies;

            printf("New Total Copies: ");
            scanf("%d", &newTotalCopies);

            if (newTotalCopies < borrowedCopies)
            {
                printf("\nError: Total copies cannot be less than currently issued copies.\n");
                fclose(file);
                return;
            }

            book.availableCopies = newTotalCopies - borrowedCopies;
            book.totalCopies = newTotalCopies;

            clearInputBuffer();

            printf("New Publication Company: ");
            readLine(book.publicationCompany, MAX_COMPANY_LEN);

            printf("New Edition: ");
            readLine(book.edition, MAX_EDITION_LEN);

            fseek(file, -(long)sizeof(Book), SEEK_CUR);
            fwrite(&book, sizeof(Book), 1, file);

            break;
        }
    }

    fclose(file);

    if (found)
    {
        printf("\nBook updated successfully.\n");
    }
    else
    {
        printf("\nBook not found or inactive.\n");
    }
}

void searchBooks(void)
{
    FILE *file = fopen(BOOKS_FILE, "rb");
    Book book;
    char keyword[150];
    int found = 0;

    if (file == NULL)
    {
        printf("\nError: Could not open books file.\n");
        return;
    }

    printf("\nSEARCH BOOKS\n");
    clearInputBuffer();

    printf("Enter ID, title, author, or category: ");
    readLine(keyword, sizeof(keyword));

    while (fread(&book, sizeof(Book), 1, file) == 1)
    {
        if (book.isActive == STATUS_ACTIVE &&
            (strstr(book.id, keyword) != NULL ||
             strstr(book.title, keyword) != NULL ||
             strstr(book.author, keyword) != NULL ||
             strstr(book.category, keyword) != NULL))
        {
            if (!found)
            {
                printBookHeader();
            }

            printBookRow(book);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\nNo matching active books found.\n");
    }
}

void viewAvailableBooks(void)
{
    FILE *file = fopen(BOOKS_FILE, "rb");
    Book book;
    int found = 0;

    if (file == NULL)
    {
        printf("\nError: Could not open books file.\n");
        return;
    }

    printf("\nAVAILABLE BOOKS\n");

    while (fread(&book, sizeof(Book), 1, file) == 1)
    {
        if (book.isActive == STATUS_ACTIVE && book.availableCopies > 0)
        {
            if (!found)
            {
                printBookHeader();
            }

            printBookRow(book);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\nNo available books found.\n");
    }
}