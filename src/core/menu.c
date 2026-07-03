#include <stdio.h>

#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/menu.h"

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
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice)
        {
            case 1:
                printf("\nAdd Book module will be added next.\n");
                break;

            case 2:
                printf("\nRemove Book module will be added later.\n");
                break;

            case 3:
                printf("\nUpdate Book module will be added later.\n");
                break;

            case 4:
                printf("\nSearch Books module will be added later.\n");
                break;

            case 5:
                printf("\nView Available Books module will be added later.\n");
                break;

            case 6:
                printf("\nRegister Student module will be added later.\n");
                break;

            case 7:
                printf("\nRemove Student module will be added later.\n");
                break;

            case 8:
                printf("\nView Registered Students module will be added later.\n");
                break;

            case 9:
                printf("\nSearch Student module will be added later.\n");
                break;

            case 10:
                printf("\nIssue Book module will be added later.\n");
                break;

            case 11:
                printf("\nReturn Book module will be added later.\n");
                break;

            case 12:
                printf("\nIssued Books module will be added later.\n");
                break;

            case 13:
                printf("\nOverdue Books module will be added later.\n");
                break;

            case 14:
                printf("\nIssue History module will be added later.\n");
                break;

            case 15:
                printf("\nFine Management module will be added later.\n");
                break;

            case 16:
                printf("\nReports module will be added later.\n");
                break;

            case 17:
                printf("\nExport CSV module will be added later.\n");
                break;

            case 18:
                printf("\nLogged out successfully.\n");
                break;

            default:
                printf("\nInvalid choice. Please select between 1 and 18.\n");
        }

    } while (choice != 18);
}