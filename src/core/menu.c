#include <stdio.h>

#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/menu.h"
#include "../../include/book.h"
#include "../../include/student.h"
#include "../../include/transaction.h"
#include "../../include/fine.h"
#include "../../include/report.h"
#include "../../include/export.h"
#include "../../include/utils.h"

void showAdminMenu(User loggedInUser)
{
    int choice;

    do
    {
        printf("\n============================================================\n");
        printf(" ADMIN DASHBOARD\n");
        printf(" Logged in as: %s (%s)\n", loggedInUser.name, loggedInUser.id);
        printf("============================================================\n");

        printf(" 1. Add Book\n");
        printf(" 2. Remove Book\n");
        printf(" 3. Update Book\n");
        printf(" 4. Search Books\n");
        printf(" 5. View Available Books\n");

        printf("\n 6. Register Student\n");
        printf(" 7. Remove Student\n");
        printf(" 8. View Registered Students\n");
        printf(" 9. Search Student\n");

        printf("\n10. Issue Book\n");
        printf("11. Return Book\n");
        printf("12. Issued Books\n");
        printf("13. Overdue Books\n");
        printf("14. Issue History\n");

        printf("\n15. Fine Management\n");
        printf("16. Reports\n");
        printf("17. Export CSV\n");
        printf("18. Logout\n");

        printf("============================================================\n");

        choice = readInt("Enter choice: ");

        switch (choice)
        {
            case 1:
                addBook();
                pauseScreen();
                break;

            case 2:
                removeBook();
                pauseScreen();
                break;

            case 3:
                updateBook();
                pauseScreen();
                break;

            case 4:
                searchBooks();
                pauseScreen();
                break;

            case 5:
                viewAvailableBooks();
                pauseScreen();
                break;

            case 6:
                registerStudent();
                pauseScreen();
                break;

            case 7:
                removeStudent();
                pauseScreen();
                break;

            case 8:
                viewRegisteredStudents();
                pauseScreen();
                break;

            case 9:
                searchStudent();
                pauseScreen();
                break;

            case 10:
                issueBook();
                pauseScreen();
                break;

            case 11:
                returnBook();
                pauseScreen();
                break;

            case 12:
                viewIssuedBooks();
                pauseScreen();
                break;

            case 13:
                viewOverdueBooks();
                pauseScreen();
                break;

            case 14:
                viewIssueHistory();
                pauseScreen();
                break;

            case 15:
                fineManagementMenu();
                break;

            case 16:
                reportsMenu();
                break;

            case 17:
                exportMenu();
                break;

            case 18:
                printf("\nLogged out successfully.\n");
                break;

            default:
                printf("\nInvalid choice. Please select between 1 and 18.\n");
                pauseScreen();
        }

    } while (choice != 18);
}

void showStudentMenu(User loggedInUser)
{
    int choice;

    do
    {
        printf("\n============================================================\n");
        printf(" STUDENT DASHBOARD\n");
        printf(" Logged in as: %s (%s)\n", loggedInUser.name, loggedInUser.id);
        printf("============================================================\n");

        printf("1. Search Books\n");
        printf("2. View Available Books\n");
        printf("3. My Borrowing History\n");
        printf("4. Logout\n");

        printf("============================================================\n");

        choice = readInt("Enter choice: ");

        switch (choice)
        {
            case 1:
                searchBooks();
                pauseScreen();
                break;

            case 2:
                viewAvailableBooks();
                pauseScreen();
                break;

            case 3:
                viewStudentBorrowingHistory(loggedInUser.id);
                pauseScreen();
                break;

            case 4:
                printf("\nLogged out successfully.\n");
                break;

            default:
                printf("\nInvalid choice. Please select between 1 and 4.\n");
                pauseScreen();
        }

    } while (choice != 4);
}