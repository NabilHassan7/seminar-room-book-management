#include <stdio.h>
#include <string.h>

#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/auth.h"

int adminExists(void)
{
    FILE *file = fopen(USERS_FILE, "rb");
    User user;

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&user, sizeof(User), 1, file) == 1)
    {
        if (strcmp(user.id, DEFAULT_ADMIN_ID) == 0 &&
            strcmp(user.role, ROLE_ADMIN) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void seedAdminAccount(void)
{
    FILE *file;
    User admin;

    if (adminExists())
    {
        return;
    }

    file = fopen(USERS_FILE, "ab");

    if (file == NULL)
    {
        printf("Error: Could not open users file for admin setup.\n");
        return;
    }

    strcpy(admin.id, DEFAULT_ADMIN_ID);
    strcpy(admin.name, DEFAULT_ADMIN_NAME);
    strcpy(admin.role, DEFAULT_ADMIN_ROLE);
    strcpy(admin.department, DEFAULT_ADMIN_DEPARTMENT);
    strcpy(admin.password, DEFAULT_ADMIN_PASSWORD);
    admin.isActive = STATUS_ACTIVE;

    fwrite(&admin, sizeof(User), 1, file);

    fclose(file);
}

int login(User *loggedInUser)
{
    FILE *file = fopen(USERS_FILE, "rb");
    User user;

    char inputId[MAX_ID_LEN];
    char inputPassword[MAX_PASSWORD_LEN];

    if (file == NULL)
    {
        printf("Error: Could not open users file.\n");
        return 0;
    }

    printf("\nLogin\n");
    printf("User ID  : ");
    scanf("%19s", inputId);

    printf("Password : ");
    scanf("%49s", inputPassword);

    while (fread(&user, sizeof(User), 1, file) == 1)
    {
        if (strcmp(user.id, inputId) == 0 &&
            strcmp(user.password, inputPassword) == 0 &&
            user.isActive == STATUS_ACTIVE)
        {
            *loggedInUser = user;
            fclose(file);
            return 1;
        }
    }

    fclose(file);

    printf("\nInvalid ID/password or inactive account.\n");
    return 0;
}