#include <stdio.h>
#include <string.h>

#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/date.h"
#include "../../include/transaction.h"
#include "../../include/fine.h"

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

static void printFineHeader(void)
{
    printf("\n%-8s %-12s %-12s %-12s %-12s %-14s %-14s\n",
           "Txn ID", "Student ID", "Book ID", "Due Date", "Return Date",
           "Overdue Days", "Fine");

    printf("------------------------------------------------------------------------------------------\n");
}

static void printFineRow(Transaction transaction)
{
    printf("%-8d %-12s %-12s ",
           transaction.transactionId,
           transaction.userId,
           transaction.bookId);

    printDate(transaction.dueDate);
    printf("   ");

    printDate(transaction.returnDate);
    printf("   ");

    printf("%-14d %.2f Taka\n",
           transaction.overdueDays,
           transaction.fineAmount);
}

void viewStudentFine(void)
{
    FILE *file = fopen(TRANSACTIONS_FILE, "rb");
    Transaction transaction;
    char studentId[MAX_ID_LEN];
    Date today = getCurrentDate();
    int found = 0;
    double totalFine = 0.0;

    if (file == NULL)
    {
        printf("\nError: Could not open transactions file.\n");
        return;
    }

    printf("\nVIEW STUDENT FINE\n");
    clearInputBuffer();

    printf("Student ID: ");
    readLine(studentId, MAX_ID_LEN);

    while (fread(&transaction, sizeof(Transaction), 1, file) == 1)
    {
        if (strcmp(transaction.userId, studentId) == 0)
        {
            if (strcmp(transaction.status, TRANSACTION_ISSUED) == 0)
            {
                transaction.overdueDays = calculateOverdueDays(transaction.dueDate, today);
                transaction.fineAmount = calculateFineAmount(transaction.overdueDays);
                transaction.finePaid = transaction.fineAmount > 0 ? FINE_UNPAID : FINE_PAID;
            }

            if (transaction.fineAmount > 0 && transaction.finePaid == FINE_UNPAID)
            {
                if (!found)
                {
                    printFineHeader();
                }

                printFineRow(transaction);
                totalFine += transaction.fineAmount;
                found = 1;
            }
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\nNo unpaid fine found for this student.\n");
    }
    else
    {
        printf("\nTotal Unpaid Fine: %.2f Taka\n", totalFine);
    }
}

void viewAllUnpaidFines(void)
{
    FILE *file = fopen(TRANSACTIONS_FILE, "rb");
    Transaction transaction;
    Date today = getCurrentDate();
    int found = 0;
    double totalFine = 0.0;

    if (file == NULL)
    {
        printf("\nError: Could not open transactions file.\n");
        return;
    }

    printf("\nALL UNPAID FINES\n");

    while (fread(&transaction, sizeof(Transaction), 1, file) == 1)
    {
        if (strcmp(transaction.status, TRANSACTION_ISSUED) == 0)
        {
            transaction.overdueDays = calculateOverdueDays(transaction.dueDate, today);
            transaction.fineAmount = calculateFineAmount(transaction.overdueDays);
            transaction.finePaid = transaction.fineAmount > 0 ? FINE_UNPAID : FINE_PAID;
        }

        if (transaction.fineAmount > 0 && transaction.finePaid == FINE_UNPAID)
        {
            if (!found)
            {
                printFineHeader();
            }

            printFineRow(transaction);
            totalFine += transaction.fineAmount;
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\nNo unpaid fines found.\n");
    }
    else
    {
        printf("\nTotal Unpaid Fine: %.2f Taka\n", totalFine);
    }
}

void markFineAsPaid(void)
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

    printf("\nMARK FINE AS PAID\n");
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
            transaction.fineAmount > 0 &&
            transaction.finePaid == FINE_UNPAID)
        {
            transaction.finePaid = FINE_PAID;

            fseek(file, -(long)sizeof(Transaction), SEEK_CUR);
            fwrite(&transaction, sizeof(Transaction), 1, file);

            found = 1;
            break;
        }
    }

    fclose(file);

    if (found)
    {
        printf("\nFine marked as paid successfully.\n");
    }
    else
    {
        printf("\nUnpaid fine transaction not found.\n");
    }
}

void fineManagementMenu(void)
{
    int choice;

    do
    {
        printf("\n============================================================\n");
        printf(" FINE MANAGEMENT\n");
        printf("============================================================\n");
        printf("1. View Student Fine\n");
        printf("2. View All Unpaid Fines\n");
        printf("3. Mark Fine as Paid\n");
        printf("4. Back\n");
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
                viewStudentFine();
                break;

            case 2:
                viewAllUnpaidFines();
                break;

            case 3:
                markFineAsPaid();
                break;

            case 4:
                printf("\nReturning to Admin Dashboard.\n");
                break;

            default:
                printf("\nInvalid choice. Please select between 1 and 4.\n");
        }

    } while (choice != 4);
}