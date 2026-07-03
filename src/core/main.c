#include <stdio.h>
#include "../../include/config.h"
#include "../../include/models.h"


void displayStartupScreen(void);

int main(void)
{
    displayStartupScreen();

    printf("\nSystem initialized successfully.\n");
    printf("Next step: Binary file initialization and admin account setup.\n\n");

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