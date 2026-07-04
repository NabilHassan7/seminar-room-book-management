#include <stdio.h>
#include <string.h>

#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/date.h"
#include "../../include/transaction.h"
#include "../../include/export.h"

static void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void writeDateCSV(FILE *file, Date date)
{
    if (isEmptyDate(date))
    {
        fprintf(file, "N/A");
    }
    else
    {
        fprintf(file, "%02d-%02d-%04d", date.day, date.month, date.year);
    }
}

void exportBooksCSV(void)
{
    FILE *dataFile = fopen(BOOKS_FILE, "rb");
    FILE *csvFile = fopen("exports/books.csv", "w");
    Book book;

    if (dataFile == NULL || csvFile == NULL)
    {
        printf("\nError: Could not export books CSV.\n");
        if (dataFile) fclose(dataFile);
        if (csvFile) fclose(csvFile);
        return;
    }

    fprintf(csvFile, "ID,Title,Author,Category,Total Copies,Available Copies,Publication Company,Edition,Status\n");

    while (fread(&book, sizeof(Book), 1, dataFile) == 1)
    {
        fprintf(csvFile, "%s,%s,%s,%s,%d,%d,%s,%s,%s\n",
                book.id,
                book.title,
                book.author,
                book.category,
                book.totalCopies,
                book.availableCopies,
                book.publicationCompany,
                book.edition,
                book.isActive == STATUS_ACTIVE ? "Active" : "Inactive");
    }

    fclose(dataFile);
    fclose(csvFile);

    printf("\nCSV exported: exports/books.csv\n");
}

void exportStudentsCSV(void)
{
    FILE *dataFile = fopen(USERS_FILE, "rb");
    FILE *csvFile = fopen("exports/students.csv", "w");
    User user;

    if (dataFile == NULL || csvFile == NULL)
    {
        printf("\nError: Could not export students CSV.\n");
        if (dataFile) fclose(dataFile);
        if (csvFile) fclose(csvFile);
        return;
    }

    fprintf(csvFile, "ID,Name,Role,Department,Status\n");

    while (fread(&user, sizeof(User), 1, dataFile) == 1)
    {
        if (strcmp(user.role, ROLE_STUDENT) == 0)
        {
            fprintf(csvFile, "%s,%s,%s,%s,%s\n",
                    user.id,
                    user.name,
                    user.role,
                    user.department,
                    user.isActive == STATUS_ACTIVE ? "Active" : "Inactive");
        }
    }

    fclose(dataFile);
    fclose(csvFile);

    printf("\nCSV exported: exports/students.csv\n");
}

void exportTransactionsCSV(void)
{
    FILE *dataFile = fopen(TRANSACTIONS_FILE, "rb");
    FILE *csvFile = fopen("exports/transactions.csv", "w");
    Transaction transaction;

    if (dataFile == NULL || csvFile == NULL)
    {
        printf("\nError: Could not export transactions CSV.\n");
        if (dataFile) fclose(dataFile);
        if (csvFile) fclose(csvFile);
        return;
    }

    fprintf(csvFile, "Transaction ID,Book ID,Student ID,Issue Date,Due Date,Return Date,Status,Overdue Days,Fine Amount,Fine Status\n");

    while (fread(&transaction, sizeof(Transaction), 1, dataFile) == 1)
    {
        fprintf(csvFile, "%d,%s,%s,",
                transaction.transactionId,
                transaction.bookId,
                transaction.userId);

        writeDateCSV(csvFile, transaction.issueDate);
        fprintf(csvFile, ",");

        writeDateCSV(csvFile, transaction.dueDate);
        fprintf(csvFile, ",");

        writeDateCSV(csvFile, transaction.returnDate);

        fprintf(csvFile, ",%s,%d,%.2f,%s\n",
                transaction.status,
                transaction.overdueDays,
                transaction.fineAmount,
                transaction.finePaid == FINE_PAID ? "Paid" : "Unpaid");
    }

    fclose(dataFile);
    fclose(csvFile);

    printf("\nCSV exported: exports/transactions.csv\n");
}

void exportIssuedBooksCSV(void)
{
    FILE *dataFile = fopen(TRANSACTIONS_FILE, "rb");
    FILE *csvFile = fopen("exports/issued_books.csv", "w");
    Transaction transaction;
    Date today = getCurrentDate();

    if (dataFile == NULL || csvFile == NULL)
    {
        printf("\nError: Could not export issued books CSV.\n");
        if (dataFile) fclose(dataFile);
        if (csvFile) fclose(csvFile);
        return;
    }

    fprintf(csvFile, "Transaction ID,Book ID,Student ID,Issue Date,Due Date,Overdue Days,Fine Amount\n");

    while (fread(&transaction, sizeof(Transaction), 1, dataFile) == 1)
    {
        if (strcmp(transaction.status, TRANSACTION_ISSUED) == 0)
        {
            transaction.overdueDays = calculateOverdueDays(transaction.dueDate, today);
            transaction.fineAmount = calculateFineAmount(transaction.overdueDays);

            fprintf(csvFile, "%d,%s,%s,",
                    transaction.transactionId,
                    transaction.bookId,
                    transaction.userId);

            writeDateCSV(csvFile, transaction.issueDate);
            fprintf(csvFile, ",");

            writeDateCSV(csvFile, transaction.dueDate);

            fprintf(csvFile, ",%d,%.2f\n",
                    transaction.overdueDays,
                    transaction.fineAmount);
        }
    }

    fclose(dataFile);
    fclose(csvFile);

    printf("\nCSV exported: exports/issued_books.csv\n");
}

void exportOverdueBooksCSV(void)
{
    FILE *dataFile = fopen(TRANSACTIONS_FILE, "rb");
    FILE *csvFile = fopen("exports/overdue_books.csv", "w");
    Transaction transaction;
    Date today = getCurrentDate();

    if (dataFile == NULL || csvFile == NULL)
    {
        printf("\nError: Could not export overdue books CSV.\n");
        if (dataFile) fclose(dataFile);
        if (csvFile) fclose(csvFile);
        return;
    }

    fprintf(csvFile, "Transaction ID,Book ID,Student ID,Issue Date,Due Date,Overdue Days,Fine Amount\n");

    while (fread(&transaction, sizeof(Transaction), 1, dataFile) == 1)
    {
        if (strcmp(transaction.status, TRANSACTION_ISSUED) == 0)
        {
            transaction.overdueDays = calculateOverdueDays(transaction.dueDate, today);
            transaction.fineAmount = calculateFineAmount(transaction.overdueDays);

            if (transaction.overdueDays > 0)
            {
                fprintf(csvFile, "%d,%s,%s,",
                        transaction.transactionId,
                        transaction.bookId,
                        transaction.userId);

                writeDateCSV(csvFile, transaction.issueDate);
                fprintf(csvFile, ",");

                writeDateCSV(csvFile, transaction.dueDate);

                fprintf(csvFile, ",%d,%.2f\n",
                        transaction.overdueDays,
                        transaction.fineAmount);
            }
        }
    }

    fclose(dataFile);
    fclose(csvFile);

    printf("\nCSV exported: exports/overdue_books.csv\n");
}

void exportUnpaidFinesCSV(void)
{
    FILE *dataFile = fopen(TRANSACTIONS_FILE, "rb");
    FILE *csvFile = fopen("exports/unpaid_fines.csv", "w");
    Transaction transaction;
    Date today = getCurrentDate();

    if (dataFile == NULL || csvFile == NULL)
    {
        printf("\nError: Could not export unpaid fines CSV.\n");
        if (dataFile) fclose(dataFile);
        if (csvFile) fclose(csvFile);
        return;
    }

    fprintf(csvFile, "Transaction ID,Student ID,Book ID,Due Date,Return Date,Overdue Days,Fine Amount,Fine Status\n");

    while (fread(&transaction, sizeof(Transaction), 1, dataFile) == 1)
    {
        if (strcmp(transaction.status, TRANSACTION_ISSUED) == 0)
        {
            transaction.overdueDays = calculateOverdueDays(transaction.dueDate, today);
            transaction.fineAmount = calculateFineAmount(transaction.overdueDays);
            transaction.finePaid = transaction.fineAmount > 0 ? FINE_UNPAID : FINE_PAID;
        }

        if (transaction.fineAmount > 0 && transaction.finePaid == FINE_UNPAID)
        {
            fprintf(csvFile, "%d,%s,%s,",
                    transaction.transactionId,
                    transaction.userId,
                    transaction.bookId);

            writeDateCSV(csvFile, transaction.dueDate);
            fprintf(csvFile, ",");

            writeDateCSV(csvFile, transaction.returnDate);

            fprintf(csvFile, ",%d,%.2f,Unpaid\n",
                    transaction.overdueDays,
                    transaction.fineAmount);
        }
    }

    fclose(dataFile);
    fclose(csvFile);

    printf("\nCSV exported: exports/unpaid_fines.csv\n");
}

void exportMenu(void)
{
    int choice;

    do
    {
        printf("\n============================================================\n");
        printf(" EXPORT CSV\n");
        printf("============================================================\n");
        printf("1. Export Books\n");
        printf("2. Export Students\n");
        printf("3. Export Transactions\n");
        printf("4. Export Issued Books\n");
        printf("5. Export Overdue Books\n");
        printf("6. Export Unpaid Fines\n");
        printf("7. Export All\n");
        printf("8. Back\n");
        printf("============================================================\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }

        switch (choice)
        {
            case 1:
                exportBooksCSV();
                break;

            case 2:
                exportStudentsCSV();
                break;

            case 3:
                exportTransactionsCSV();
                break;

            case 4:
                exportIssuedBooksCSV();
                break;

            case 5:
                exportOverdueBooksCSV();
                break;

            case 6:
                exportUnpaidFinesCSV();
                break;

            case 7:
                exportBooksCSV();
                exportStudentsCSV();
                exportTransactionsCSV();
                exportIssuedBooksCSV();
                exportOverdueBooksCSV();
                exportUnpaidFinesCSV();
                break;

            case 8:
                printf("\nReturning to Admin Dashboard.\n");
                break;

            default:
                printf("\nInvalid choice. Please select between 1 and 8.\n");
        }

    } while (choice != 8);
}