#include <stdio.h>
#include <string.h>

#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/file.h"
#include "../../include/auth.h"
#include "../../include/menu.h"

void displayStartupScreen(void);

int main(void)
{
    User loggedInUser;

    displayStartupScreen();

    initializeSystemFiles();
    seedAdminAccount();

    if (login(&loggedInUser))
    {
        printf("\nLogin successful.\n");
        printf("Welcome, %s\n", loggedInUser.name);
        printf("Role: %s\n", loggedInUser.role);

        if (strcmp(loggedInUser.role, ROLE_ADMIN) == 0)
        {
            showAdminMenu(loggedInUser);
        }
        else if (strcmp(loggedInUser.role, ROLE_STUDENT) == 0)
        {
            showStudentMenu(loggedInUser);
        }
    }
    else
    {
        printf("\nAccess denied.\n");
    }

    return 0;
}

void displayStartupScreen(void)
{
    printf("============================================================\n");
    printf(" %s\n", APP_NAME);
    printf(" %s\n", DEPARTMENT_NAME);
    printf(" %s\n", UNIVERSITY_NAME);
    printf("============================================================\n");
}