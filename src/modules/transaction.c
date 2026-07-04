#include <stdio.h>
#include <string.h>

#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/transaction.h"
#include "../../include/date.h"
#include "../../include/book.h"
#include "../../include/student.h"

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

static void printTransactionHeader(void)
{
    printf("\n%-8s %-12s %-12s %-12s %-12s %-12s %-12s %-14s %-12s\n",
           "Txn ID", "Book ID", "Student ID", "Issue Date", "Due Date",
           "Return Date", "Status", "Overdue Days", "Fine");

    printf("------------------------------------------------------------------------------------------------------------------------\n");
}

static void printTransactionRow(Transaction transaction)
{
    printf("%-8d %-12s %-12s ",
           transaction.transactionId,
           transaction.bookId,
           transaction.userId);

    printDate(transaction.issueDate);
    printf("   ");

    printDate(transaction.dueDate);
    printf("   ");

    printDate(transaction.returnDate);
    printf("   ");

    printf("%-12s %-14d %.2f Taka\n",
           transaction.status,
           transaction.overdueDays,
           transaction.fineAmount);
}

int calculateOverdueDays(Date dueDate, Date checkDate)
{
    if (compareDates(checkDate, dueDate) <= 0)
    {
        return 0;
    }

    return daysBetween(dueDate, checkDate);
}

double calculateFineAmount(int overdueDays)
{
    if (overdueDays <= 0)
    {
        return 0.0;
    }

    return overdueDays * FINE_PER_DAY;
}

int getNextTransactionId(void)
{
    FILE *file = fopen(TRANSACTIONS_FILE, "rb");
    Transaction transaction;
    int maxId = 0;

    if (file == NULL)
    {
        return 1;
    }

    while (fread(&transaction, sizeof(Transaction), 1, file) == 1)
    {
        if (transaction.transactionId > maxId)
        {
            maxId = transaction.transactionId;
        }
    }

    fclose(file);
    return maxId + 1;
}

int hasActiveIssue(const char *studentId, const char *bookId)
{
    FILE *file = fopen(TRANSACTIONS_FILE, "rb");
    Transaction transaction;

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&transaction, sizeof(Transaction), 1, file) == 1)
    {
        if (strcmp(transaction.userId, studentId) == 0 &&
            strcmp(transaction.bookId, bookId) == 0 &&
            strcmp(transaction.status, TRANSACTION_ISSUED) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int hasUnpaidFine(const char *studentId)
{
    FILE *file = fopen(TRANSACTIONS_FILE, "rb");
    Transaction transaction;
    Date today = getCurrentDate();
    int overdueDays;
    double fineAmount;

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&transaction, sizeof(Transaction), 1, file) == 1)
    {
        if (strcmp(transaction.userId, studentId) == 0)
        {
            if (strcmp(transaction.status, TRANSACTION_ISSUED) == 0)
            {
                overdueDays = calculateOverdueDays(transaction.dueDate, today);
                fineAmount = calculateFineAmount(overdueDays);

                if (fineAmount > 0)
                {
                    fclose(file);
                    return 1;
                }
            }

            if (strcmp(transaction.status, TRANSACTION_RETURNED) == 0 &&
                transaction.fineAmount > 0 &&
                transaction.finePaid == FINE_UNPAID)
            {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

static int reduceAvailableCopy(const char *bookId)
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

static int increaseAvailableCopy(const char *bookId)
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
            book.availableCopies < book.totalCopies)
        {
            book.availableCopies++;

            fseek(file, -(long)sizeof(Book), SEEK_CUR);
            fwrite(&book, sizeof(Book), 1, file);

            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void issueBook(void)
{
    FILE *file;
    Transaction transaction;

    char studentId[MAX_ID_LEN];
    char bookId[MAX_ID_LEN];

    printf("\nISSUE BOOK\n");

    clearInputBuffer();

    printf("Student ID: ");
    readLine(studentId, MAX_ID_LEN);

    if (!studentExistsById(studentId))
    {
        printf("\nError: Active student not found.\n");
        return;
    }

    if (hasUnpaidFine(studentId))
    {
        printf("\nError: Student has unpaid overdue fine.\n");
        printf("Book issue is blocked until the fine is paid.\n");
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

    if (!reduceAvailableCopy(bookId))
    {
        printf("\nError: Book is not available for issue.\n");
        return;
    }

    transaction.transactionId = getNextTransactionId();
    strcpy(transaction.bookId, bookId);
    strcpy(transaction.userId, studentId);

    transaction.issueDate = getCurrentDate();
    transaction.dueDate = addDays(transaction.issueDate, LOAN_PERIOD_DAYS);

    transaction.returnDate.day = 0;
    transaction.returnDate.month = 0;
    transaction.returnDate.year = 0;

    strcpy(transaction.status, TRANSACTION_ISSUED);

    transaction.overdueDays = 0;
    transaction.fineAmount = 0.0;
    transaction.finePaid = FINE_PAID;

    file = fopen(TRANSACTIONS_FILE, "ab");

    if (file == NULL)
    {
        increaseAvailableCopy(bookId);
        printf("\nError: Could not open transactions file.\n");
        return;
    }

    fwrite(&transaction, sizeof(Transaction), 1, file);
    fclose(file);

    printf("\nBook issued successfully.\n");
    printf("Transaction ID: %d\n", transaction.transactionId);
    printf("Issue Date    : ");
    printDate(transaction.issueDate);
    printf("\nDue Date      : ");
    printDate(transaction.dueDate);
    printf("\n");
}

void returnBook(void)
{
    FILE *file = fopen(TRANSACTIONS_FILE, "rb+");
    Transaction transaction;

    int transactionId;
    int found = 0;

    if (file == NULL)
    {
        printf("\nError: Could not open transactions file.\n");
        return;
    }

    printf("\nRETURN BOOK\n");
    printf("Transaction ID: ");

    if (scanf("%d", &transactionId) != 1)
    {
        printf("\nInvalid transaction ID.\n");
        clearInputBuffer();
        fclose(file);
        return;
    }

    while (fread(&transaction, sizeof(Transaction), 1, file) == 1)
    {
        if (transaction.transactionId == transactionId &&
            strcmp(transaction.status, TRANSACTION_ISSUED) == 0)
        {
            transaction.returnDate = getCurrentDate();

            transaction.overdueDays = calculateOverdueDays(
                transaction.dueDate,
                transaction.returnDate
            );

            transaction.fineAmount = calculateFineAmount(transaction.overdueDays);

            if (transaction.fineAmount > 0)
            {
                transaction.finePaid = FINE_UNPAID;
            }
            else
            {
                transaction.finePaid = FINE_PAID;
            }

            strcpy(transaction.status, TRANSACTION_RETURNED);

            fseek(file, -(long)sizeof(Transaction), SEEK_CUR);
            fwrite(&transaction, sizeof(Transaction), 1, file);

            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\nActive issued transaction not found.\n");
        return;
    }

    if (!increaseAvailableCopy(transaction.bookId))
    {
        printf("\nWarning: Book returned, but available copy update failed.\n");
    }

    printf("\nBook returned successfully.\n");
    printf("Book ID       : %s\n", transaction.bookId);
    printf("Student ID    : %s\n", transaction.userId);
    printf("Return Date   : ");
    printDate(transaction.returnDate);
    printf("\nOverdue Days  : %d\n", transaction.overdueDays);
    printf("Fine Amount   : %.2f Taka\n", transaction.fineAmount);

    if (transaction.fineAmount > 0)
    {
        printf("Fine Status   : Unpaid\n");
    }
    else
    {
        printf("Fine Status   : No fine\n");
    }
}

void viewIssuedBooks(void)
{
    FILE *file = fopen(TRANSACTIONS_FILE, "rb");
    Transaction transaction;
    Date today = getCurrentDate();
    int found = 0;

    if (file == NULL)
    {
        printf("\nError: Could not open transactions file.\n");
        return;
    }

    printf("\nISSUED BOOKS\n");

    while (fread(&transaction, sizeof(Transaction), 1, file) == 1)
    {
        if (strcmp(transaction.status, TRANSACTION_ISSUED) == 0)
        {
            transaction.overdueDays = calculateOverdueDays(transaction.dueDate, today);
            transaction.fineAmount = calculateFineAmount(transaction.overdueDays);

            if (!found)
            {
                printTransactionHeader();
            }

            printTransactionRow(transaction);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\nNo issued books found.\n");
    }
}

void viewOverdueBooks(void)
{
    FILE *file = fopen(TRANSACTIONS_FILE, "rb");
    Transaction transaction;
    Date today = getCurrentDate();
    int found = 0;

    if (file == NULL)
    {
        printf("\nError: Could not open transactions file.\n");
        return;
    }

    printf("\nOVERDUE BOOKS\n");

    while (fread(&transaction, sizeof(Transaction), 1, file) == 1)
    {
        if (strcmp(transaction.status, TRANSACTION_ISSUED) == 0)
        {
            transaction.overdueDays = calculateOverdueDays(transaction.dueDate, today);
            transaction.fineAmount = calculateFineAmount(transaction.overdueDays);

            if (transaction.overdueDays > 0)
            {
                if (!found)
                {
                    printTransactionHeader();
                }

                printTransactionRow(transaction);
                found = 1;
            }
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\nNo overdue books found.\n");
    }
}

void viewIssueHistory(void)
{
    FILE *file = fopen(TRANSACTIONS_FILE, "rb");
    Transaction transaction;
    int found = 0;

    if (file == NULL)
    {
        printf("\nError: Could not open transactions file.\n");
        return;
    }

    printf("\nISSUE HISTORY\n");

    while (fread(&transaction, sizeof(Transaction), 1, file) == 1)
    {
        if (!found)
        {
            printTransactionHeader();
        }

        printTransactionRow(transaction);
        found = 1;
    }

    fclose(file);

    if (!found)
    {
        printf("\nNo issue history found.\n");
    }
}

void viewStudentBorrowingHistory(const char *studentId)
{
    FILE *file = fopen(TRANSACTIONS_FILE, "rb");
    Transaction transaction;
    Date today = getCurrentDate();
    int found = 0;

    if (file == NULL)
    {
        printf("\nError: Could not open transactions file.\n");
        return;
    }

    printf("\nMY BORROWING HISTORY\n");

    while (fread(&transaction, sizeof(Transaction), 1, file) == 1)
    {
        if (strcmp(transaction.userId, studentId) == 0)
        {
            if (strcmp(transaction.status, TRANSACTION_ISSUED) == 0)
            {
                transaction.overdueDays = calculateOverdueDays(transaction.dueDate, today);
                transaction.fineAmount = calculateFineAmount(transaction.overdueDays);
            }

            if (!found)
            {
                printTransactionHeader();
            }

            printTransactionRow(transaction);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\nNo borrowing history found.\n");
    }
}