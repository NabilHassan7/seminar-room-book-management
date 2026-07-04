#include <stdio.h>
#include <string.h>

#include "../../include/utils.h"

void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void readLine(char *buffer, int size)
{
    if (fgets(buffer, size, stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

int readInt(const char *prompt)
{
    int value;

    while (1)
    {
        printf("%s", prompt);

        if (scanf("%d", &value) == 1)
        {
            clearInputBuffer();
            return value;
        }

        printf("Invalid input. Please enter a number.\n");
        clearInputBuffer();
    }
}

int readPositiveInt(const char *prompt)
{
    int value;

    while (1)
    {
        value = readInt(prompt);

        if (value > 0)
        {
            return value;
        }

        printf("Value must be greater than zero.\n");
    }
}

void printDivider(int length)
{
    int i;

    for (i = 0; i < length; i++)
    {
        printf("-");
    }

    printf("\n");
}

void pauseScreen(void)
{
    printf("\nPress Enter to continue...");
    getchar();
}