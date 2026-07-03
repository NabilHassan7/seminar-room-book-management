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