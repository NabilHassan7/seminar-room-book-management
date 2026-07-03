#include <stdio.h>
#include <direct.h>
#include <sys/stat.h>

#include "../../include/config.h"
#include "../../include/file.h"

void ensureDirectoryExists(const char *directoryPath)
{
    struct _stat st;

    if (_stat(directoryPath, &st) != 0)
    {
        _mkdir(directoryPath);
    }
}

void ensureBinaryFileExists(const char *filePath)
{
    FILE *file = fopen(filePath, "rb");

    if (file == NULL)
    {
        file = fopen(filePath, "wb");

        if (file == NULL)
        {
            printf("Error: Could not create file: %s\n", filePath);
            return;
        }
    }

    fclose(file);
}

void initializeSystemFiles(void)
{
    ensureDirectoryExists("data");
    ensureDirectoryExists("reports");
    ensureDirectoryExists("exports");
    ensureDirectoryExists("build");

    ensureBinaryFileExists(BOOKS_FILE);
    ensureBinaryFileExists(USERS_FILE);
    ensureBinaryFileExists(TRANSACTIONS_FILE);
}