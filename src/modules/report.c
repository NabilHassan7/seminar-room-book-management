#include <stdio.h>
#include <string.h>

#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/date.h"
#include "../../include/transaction.h"
#include "../../include/report.h"

static void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void writeDate(FILE *file, Date date)
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

void generateAvailableBooksReport(void)
{
    FILE *dataFile = fopen(BOOKS_FILE, "rb");
    FILE *reportFile = fopen("reports/available_books_report.txt", "w");
    Book book;
    int found = 0;

    if (dataFile == NULL || reportFile == NULL)
    {
        printf("\nError: Could not generate available books report.\n");
        if (dataFile) fclose(dataFile);
        if (reportFile) fclose(reportFile);
        return;
    }

    fprintf(reportFile, "AVAILABLE BOOKS REPORT\n");
    fprintf(reportFile, "============================================================\n\n");

    fprintf(reportFile, "%-12s %-30s %-22s %-18s %-10s %-10s\n",
            "ID", "Title", "Author", "Category", "Total", "Available");

    fprintf(reportFile, "----------------------------------------------------------------------------------------------------\n");

    while (fread(&book, sizeof(Book), 1, dataFile) == 1)
    {
        if (book.isActive == STATUS_ACTIVE && book.availableCopies > 0)
        {
            fprintf(reportFile, "%-12s %-30s %-22s %-18s %-10d %-10d\n",
                    book.id,
                    book.title,
                    book.author,
                    book.category,
                    book.totalCopies,
                    book.availableCopies);

            found = 1;
        }
    }

    if (!found)
    {
        fprintf(reportFile, "\nNo available books found.\n");
    }

    fclose(dataFile);
    fclose(reportFile);

    printf("\nReport generated: reports/available_books_report.txt\n");
}

void generateIssuedBooksReport(void)
{
    FILE *dataFile = fopen(TRANSACTIONS_FILE, "rb");
    FILE *reportFile = fopen("reports/issued_books_report.txt", "w");
    Transaction transaction;
    Date today = getCurrentDate();
    int found = 0;

    if (dataFile == NULL || reportFile == NULL)
    {
        printf("\nError: Could not generate issued books report.\n");
        if (dataFile) fclose(dataFile);
        if (reportFile) fclose(reportFile);
        return;
    }

    fprintf(reportFile, "ISSUED BOOKS REPORT\n");
    fprintf(reportFile, "============================================================\n\n");

    fprintf(reportFile, "%-8s %-12s %-12s %-12s %-12s %-14s %-12s\n",
            "Txn ID", "Book ID", "Student ID", "Issue Date", "Due Date", "Overdue Days", "Fine");

    fprintf(reportFile, "-----------------------------------------------------------------------------------------------\n");

    while (fread(&transaction, sizeof(Transaction), 1, dataFile) == 1)
    {
        if (strcmp(transaction.status, TRANSACTION_ISSUED) == 0)
        {
            transaction.overdueDays = calculateOverdueDays(transaction.dueDate, today);
            transaction.fineAmount = calculateFineAmount(transaction.overdueDays);

            fprintf(reportFile, "%-8d %-12s %-12s ",
                    transaction.transactionId,
                    transaction.bookId,
                    transaction.userId);

            writeDate(reportFile, transaction.issueDate);
            fprintf(reportFile, "   ");

            writeDate(reportFile, transaction.dueDate);
            fprintf(reportFile, "   ");

            fprintf(reportFile, "%-14d %.2f Taka\n",
                    transaction.overdueDays,
                    transaction.fineAmount);

            found = 1;
        }
    }

    if (!found)
    {
        fprintf(reportFile, "\nNo issued books found.\n");
    }

    fclose(dataFile);
    fclose(reportFile);

    printf("\nReport generated: reports/issued_books_report.txt\n");
}

void generateOverdueBooksReport(void)
{
    FILE *dataFile = fopen(TRANSACTIONS_FILE, "rb");
    FILE *reportFile = fopen("reports/overdue_books_report.txt", "w");
    Transaction transaction;
    Date today = getCurrentDate();
    int found = 0;

    if (dataFile == NULL || reportFile == NULL)
    {
        printf("\nError: Could not generate overdue books report.\n");
        if (dataFile) fclose(dataFile);
        if (reportFile) fclose(reportFile);
        return;
    }

    fprintf(reportFile, "OVERDUE BOOKS REPORT\n");
    fprintf(reportFile, "============================================================\n\n");

    fprintf(reportFile, "%-8s %-12s %-12s %-12s %-12s %-14s %-12s\n",
            "Txn ID", "Book ID", "Student ID", "Issue Date", "Due Date", "Overdue Days", "Fine");

    fprintf(reportFile, "-----------------------------------------------------------------------------------------------\n");

    while (fread(&transaction, sizeof(Transaction), 1, dataFile) == 1)
    {
        if (strcmp(transaction.status, TRANSACTION_ISSUED) == 0)
        {
            transaction.overdueDays = calculateOverdueDays(transaction.dueDate, today);
            transaction.fineAmount = calculateFineAmount(transaction.overdueDays);

            if (transaction.overdueDays > 0)
            {
                fprintf(reportFile, "%-8d %-12s %-12s ",
                        transaction.transactionId,
                        transaction.bookId,
                        transaction.userId);

                writeDate(reportFile, transaction.issueDate);
                fprintf(reportFile, "   ");

                writeDate(reportFile, transaction.dueDate);
                fprintf(reportFile, "   ");

                fprintf(reportFile, "%-14d %.2f Taka\n",
                        transaction.overdueDays,
                        transaction.fineAmount);

                found = 1;
            }
        }
    }

    if (!found)
    {
        fprintf(reportFile, "\nNo overdue books found.\n");
    }

    fclose(dataFile);
    fclose(reportFile);

    printf("\nReport generated: reports/overdue_books_report.txt\n");
}

void generateCategoryWiseReport(void)
{
    FILE *dataFile = fopen(BOOKS_FILE, "rb");
    FILE *reportFile = fopen("reports/category_wise_report.txt", "w");
    Book book;
    int found = 0;

    if (dataFile == NULL || reportFile == NULL)
    {
        printf("\nError: Could not generate category-wise report.\n");
        if (dataFile) fclose(dataFile);
        if (reportFile) fclose(reportFile);
        return;
    }

    fprintf(reportFile, "CATEGORY-WISE BOOK REPORT\n");
    fprintf(reportFile, "============================================================\n\n");

    fprintf(reportFile, "%-18s %-12s %-30s %-22s %-10s %-10s\n",
            "Category", "Book ID", "Title", "Author", "Total", "Available");

    fprintf(reportFile, "----------------------------------------------------------------------------------------------------\n");

    while (fread(&book, sizeof(Book), 1, dataFile) == 1)
    {
        if (book.isActive == STATUS_ACTIVE)
        {
            fprintf(reportFile, "%-18s %-12s %-30s %-22s %-10d %-10d\n",
                    book.category,
                    book.id,
                    book.title,
                    book.author,
                    book.totalCopies,
                    book.availableCopies);

            found = 1;
        }
    }

    if (!found)
    {
        fprintf(reportFile, "\nNo active books found.\n");
    }

    fclose(dataFile);
    fclose(reportFile);

    printf("\nReport generated: reports/category_wise_report.txt\n");
}

void generateIssueHistoryReport(void)
{
    FILE *dataFile = fopen(TRANSACTIONS_FILE, "rb");
    FILE *reportFile = fopen("reports/issue_history_report.txt", "w");
    Transaction transaction;
    int found = 0;

    if (dataFile == NULL || reportFile == NULL)
    {
        printf("\nError: Could not generate issue history report.\n");
        if (dataFile) fclose(dataFile);
        if (reportFile) fclose(reportFile);
        return;
    }

    fprintf(reportFile, "ISSUE HISTORY REPORT\n");
    fprintf(reportFile, "============================================================\n\n");

    fprintf(reportFile, "%-8s %-12s %-12s %-12s %-12s %-12s %-12s %-14s %-12s\n",
            "Txn ID", "Book ID", "Student ID", "Issue Date", "Due Date",
            "Return Date", "Status", "Overdue Days", "Fine");

    fprintf(reportFile, "------------------------------------------------------------------------------------------------------------------------\n");

    while (fread(&transaction, sizeof(Transaction), 1, dataFile) == 1)
    {
        fprintf(reportFile, "%-8d %-12s %-12s ",
                transaction.transactionId,
                transaction.bookId,
                transaction.userId);

        writeDate(reportFile, transaction.issueDate);
        fprintf(reportFile, "   ");

        writeDate(reportFile, transaction.dueDate);
        fprintf(reportFile, "   ");

        writeDate(reportFile, transaction.returnDate);
        fprintf(reportFile, "   ");

        fprintf(reportFile, "%-12s %-14d %.2f Taka\n",
                transaction.status,
                transaction.overdueDays,
                transaction.fineAmount);

        found = 1;
    }

    if (!found)
    {
        fprintf(reportFile, "\nNo issue history found.\n");
    }

    fclose(dataFile);
    fclose(reportFile);

    printf("\nReport generated: reports/issue_history_report.txt\n");
}

void generateUnpaidFineReport(void)
{
    FILE *dataFile = fopen(TRANSACTIONS_FILE, "rb");
    FILE *reportFile = fopen("reports/unpaid_fine_report.txt", "w");
    Transaction transaction;
    Date today = getCurrentDate();
    int found = 0;
    double totalFine = 0.0;

    if (dataFile == NULL || reportFile == NULL)
    {
        printf("\nError: Could not generate unpaid fine report.\n");
        if (dataFile) fclose(dataFile);
        if (reportFile) fclose(reportFile);
        return;
    }

    fprintf(reportFile, "UNPAID FINE REPORT\n");
    fprintf(reportFile, "============================================================\n\n");

    fprintf(reportFile, "%-8s %-12s %-12s %-12s %-12s %-14s %-12s\n",
            "Txn ID", "Student ID", "Book ID", "Due Date", "Return Date", "Overdue Days", "Fine");

    fprintf(reportFile, "-----------------------------------------------------------------------------------------------\n");

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
            fprintf(reportFile, "%-8d %-12s %-12s ",
                    transaction.transactionId,
                    transaction.userId,
                    transaction.bookId);

            writeDate(reportFile, transaction.dueDate);
            fprintf(reportFile, "   ");

            writeDate(reportFile, transaction.returnDate);
            fprintf(reportFile, "   ");

            fprintf(reportFile, "%-14d %.2f Taka\n",
                    transaction.overdueDays,
                    transaction.fineAmount);

            totalFine += transaction.fineAmount;
            found = 1;
        }
    }

    if (!found)
    {
        fprintf(reportFile, "\nNo unpaid fines found.\n");
    }
    else
    {
        fprintf(reportFile, "\nTotal Unpaid Fine: %.2f Taka\n", totalFine);
    }

    fclose(dataFile);
    fclose(reportFile);

    printf("\nReport generated: reports/unpaid_fine_report.txt\n");
}

void reportsMenu(void)
{
    int choice;

    do
    {
        printf("\n============================================================\n");
        printf(" REPORTS\n");
        printf("============================================================\n");
        printf("1. Available Books Report\n");
        printf("2. Issued Books Report\n");
        printf("3. Overdue Books Report\n");
        printf("4. Category-wise Book Report\n");
        printf("5. Issue History Report\n");
        printf("6. Unpaid Fine Report\n");
        printf("7. Generate All Reports\n");
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
                generateAvailableBooksReport();
                break;

            case 2:
                generateIssuedBooksReport();
                break;

            case 3:
                generateOverdueBooksReport();
                break;

            case 4:
                generateCategoryWiseReport();
                break;

            case 5:
                generateIssueHistoryReport();
                break;

            case 6:
                generateUnpaidFineReport();
                break;

            case 7:
                generateAvailableBooksReport();
                generateIssuedBooksReport();
                generateOverdueBooksReport();
                generateCategoryWiseReport();
                generateIssueHistoryReport();
                generateUnpaidFineReport();
                break;

            case 8:
                printf("\nReturning to Admin Dashboard.\n");
                break;

            default:
                printf("\nInvalid choice. Please select between 1 and 8.\n");
        }

    } while (choice != 8);
}