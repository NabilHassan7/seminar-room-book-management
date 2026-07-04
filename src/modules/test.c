#include <stdio.h>
#include <string.h>

#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/date.h"
#include "../../include/book.h"
#include "../../include/student.h"
#include "../../include/transaction.h"
#include "../../include/test.h"
#include "../../include/utils.h"

static int reduceAvailableCopyForTest(const char *bookId)
{
    FILE *file = fopen(BOOKS_FILE, "rb+");
    Book book;

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&book, sizeof(Book), 1, file) == 1)
    {
        if (strcmp(book.id, bookId) == 0 &&
            book.isActive == STATUS_ACTIVE &&
            book.availableCopies > 0)
        {
            book.availableCopies--;

            fseek(file, -(long)sizeof(Book), SEEK_CUR);
            fwrite(&book, sizeof(Book), 1, file);

            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

static Date readDateFromUser(const char *label)
{
    Date date;

    printf("%s\n", label);
    date.day = readInt("Day   : ");
    date.month = readInt("Month : ");
    date.year = readInt("Year  : ");

    return date;
}

void createOverdueTestTransaction(void)
{
    FILE *file;
    Transaction transaction;

    char studentId[MAX_ID_LEN];
    char bookId[MAX_ID_LEN];

    printf("\nCREATE OVERDUE TEST TRANSACTION\n");

    printf("Student ID: ");
    readLine(studentId, MAX_ID_LEN);

    if (!studentExistsById(studentId))
    {
        printf("\nError: Active student not found.\n");
        return;
    }

    if (hasUnpaidFine(studentId))
    {
        printf("\nError: Student already has unpaid fine.\n");
        printf("Clear existing fine before creating a new test transaction.\n");
        return;
    }

    printf("Book ID: ");
    readLine(bookId, MAX_ID_LEN);

    if (!bookExistsById(bookId))
    {
        printf("\nError: Active book not found.\n");
        return;
    }

    if (hasActiveIssue(studentId, bookId))
    {
        printf("\nError: This student already has this book issued.\n");
        return;
    }

    transaction.issueDate = readDateFromUser("Enter custom issue date:");

    transaction.dueDate = addDays(transaction.issueDate, LOAN_PERIOD_DAYS);

    transaction.transactionId = getNextTransactionId();
    strcpy(transaction.bookId, bookId);
    strcpy(transaction.userId, studentId);

    transaction.returnDate.day = 0;
    transaction.returnDate.month = 0;
    transaction.returnDate.year = 0;

    strcpy(transaction.status, TRANSACTION_ISSUED);

    transaction.overdueDays = 0;
    transaction.fineAmount = 0.0;
    transaction.finePaid = FINE_PAID;

    if (!reduceAvailableCopyForTest(bookId))
    {
        printf("\nError: Book is not available for issue.\n");
        return;
    }

    file = fopen(TRANSACTIONS_FILE, "ab");

    if (file == NULL)
    {
        printf("\nError: Could not open transactions file.\n");
        return;
    }

    fwrite(&transaction, sizeof(Transaction), 1, file);
    fclose(file);

    printf("\nTest transaction created successfully.\n");
    printf("Transaction ID : %d\n", transaction.transactionId);
    printf("Issue Date     : ");
    printDate(transaction.issueDate);
    printf("\nDue Date       : ");
    printDate(transaction.dueDate);
    printf("\n");
}

void testModeMenu(void)
{
    int choice;

    do
    {
        printf("\n============================================================\n");
        printf(" TEST MODE\n");
        printf("============================================================\n");
        printf("1. Create Overdue Test Transaction\n");
        printf("2. Back\n");
        printf("============================================================\n");

        choice = readInt("Enter choice: ");

        switch (choice)
        {
            case 1:
                createOverdueTestTransaction();
                pauseScreen();
                break;

            case 2:
                printf("\nReturning to Admin Dashboard.\n");
                break;

            default:
                printf("\nInvalid choice. Please select between 1 and 2.\n");
                pauseScreen();
        }

    } while (choice != 2);
}