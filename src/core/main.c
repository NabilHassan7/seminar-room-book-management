#include <stdio.h>
#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/file.h"

void displayStartupScreen(void);

int main(void)
{
    displayStartupScreen();

    initializeSystemFiles();

    printf("\nSystem files initialized successfully.\n");
    printf("Next step: Admin account setup.\n\n");

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