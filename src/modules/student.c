#include <stdio.h>
#include <string.h>

#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/student.h"

static void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void readLine(char *buffer, int size)
{
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

static void printStudentHeader(void)
{
    printf("\n%-15s %-30s %-15s %-30s %-10s\n",
           "ID", "Name", "Role", "Department", "Status");

    printf("--------------------------------------------------------------------------------------------------------\n");
}

static void printStudentRow(User user)
{
    printf("%-15s %-30s %-15s %-30s %-10s\n",
           user.id,
           user.name,
           user.role,
           user.department,
           user.isActive == STATUS_ACTIVE ? "Active" : "Inactive");
}

int studentExistsById(const char *studentId)
{
    FILE *file = fopen(USERS_FILE, "rb");
    User user;

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&user, sizeof(User), 1, file) == 1)
    {
        if (strcmp(user.id, studentId) == 0 &&
            strcmp(user.role, ROLE_STUDENT) == 0 &&
            user.isActive == STATUS_ACTIVE)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void registerStudent(void)
{
    FILE *file;
    User student;

    printf("\nREGISTER STUDENT\n");

    clearInputBuffer();

    printf("Student ID: ");
    readLine(student.id, MAX_ID_LEN);

    if (studentExistsById(student.id))
    {
        printf("\nError: Active student with this ID already exists.\n");
        return;
    }

    printf("Name: ");
    readLine(student.name, MAX_NAME_LEN);

    strcpy(student.role, ROLE_STUDENT);

    printf("Department: ");
    readLine(student.department, MAX_DEPARTMENT_LEN);

    printf("Password: ");
    readLine(student.password, MAX_PASSWORD_LEN);

    student.isActive = STATUS_ACTIVE;

    file = fopen(USERS_FILE, "ab");

    if (file == NULL)
    {
        printf("\nError: Could not open users file.\n");
        return;
    }

    fwrite(&student, sizeof(User), 1, file);
    fclose(file);

    printf("\nStudent registered successfully.\n");
}

void removeStudent(void)
{
    FILE *file = fopen(USERS_FILE, "rb+");
    User user;
    char studentId[MAX_ID_LEN];
    int found = 0;

    if (file == NULL)
    {
        printf("\nError: Could not open users file.\n");
        return;
    }

    printf("\nREMOVE STUDENT\n");

    clearInputBuffer();

    printf("Enter Student ID: ");
    readLine(studentId, MAX_ID_LEN);

    while (fread(&user, sizeof(User), 1, file) == 1)
    {
        if (strcmp(user.id, studentId) == 0 &&
            strcmp(user.role, ROLE_STUDENT) == 0 &&
            user.isActive == STATUS_ACTIVE)
        {
            user.isActive = STATUS_INACTIVE;

            fseek(file, -(long)sizeof(User), SEEK_CUR);
            fwrite(&user, sizeof(User), 1, file);

            found = 1;
            break;
        }
    }

    fclose(file);

    if (found)
    {
        printf("\nStudent marked as inactive successfully.\n");
    }
    else
    {
        printf("\nStudent not found or already inactive.\n");
    }
}

void viewRegisteredStudents(void)
{
    FILE *file = fopen(USERS_FILE, "rb");
    User user;
    int found = 0;

    if (file == NULL)
    {
        printf("\nError: Could not open users file.\n");
        return;
    }

    printf("\nREGISTERED STUDENTS\n");

    while (fread(&user, sizeof(User), 1, file) == 1)
    {
        if (strcmp(user.role, ROLE_STUDENT) == 0 &&
            user.isActive == STATUS_ACTIVE)
        {
            if (!found)
            {
                printStudentHeader();
            }

            printStudentRow(user);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\nNo active registered students found.\n");
    }
}

void searchStudent(void)
{
    FILE *file = fopen(USERS_FILE, "rb");
    User user;
    char keyword[MAX_NAME_LEN];
    int found = 0;

    if (file == NULL)
    {
        printf("\nError: Could not open users file.\n");
        return;
    }

    printf("\nSEARCH STUDENT\n");

    clearInputBuffer();

    printf("Enter ID, name, or department: ");
    readLine(keyword, sizeof(keyword));

    while (fread(&user, sizeof(User), 1, file) == 1)
    {
        if (strcmp(user.role, ROLE_STUDENT) == 0 &&
            user.isActive == STATUS_ACTIVE &&
            (strstr(user.id, keyword) != NULL ||
             strstr(user.name, keyword) != NULL ||
             strstr(user.department, keyword) != NULL))
        {
            if (!found)
            {
                printStudentHeader();
            }

            printStudentRow(user);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printf("\nNo matching active students found.\n");
    }
}