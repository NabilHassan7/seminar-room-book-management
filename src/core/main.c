#include <stdio.h>

#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/file.h"
#include "../../include/auth.h"

void displayStartupScreen(void);

int main(void)
{
    displayStartupScreen();

    initializeSystemFiles();
    seedAdminAccount();

    printf("\nSystem files initialized successfully.\n");
    printf("Admin account setup completed.\n");
    printf("\nDefault Admin Login:\n");
    printf("ID       : %s\n", DEFAULT_ADMIN_ID);
    printf("Password : %s\n", DEFAULT_ADMIN_PASSWORD);

    printf("\nNext step: Build authentication login flow.\n\n");

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