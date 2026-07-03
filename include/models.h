#ifndef MODELS_H
#define MODELS_H

#include "config.h"

typedef struct
{
    int day;
    int month;
    int year;
} Date;

typedef struct
{
    char id[MAX_ID_LEN];
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    char category[MAX_CATEGORY_LEN];

    int totalCopies;
    int availableCopies;

    char publicationCompany[MAX_COMPANY_LEN];
    char edition[MAX_EDITION_LEN];

    int isActive;
} Book;

typedef struct
{
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char role[MAX_ROLE_LEN];
    char department[MAX_DEPARTMENT_LEN];
    char password[MAX_PASSWORD_LEN];

    int isActive;
} User;

typedef struct
{
    int transactionId;

    char bookId[MAX_ID_LEN];
    char userId[MAX_ID_LEN];

    Date issueDate;
    Date dueDate;
    Date returnDate;

    char status[MAX_STATUS_LEN];

    int overdueDays;
    double fineAmount;
    int finePaid;

} Transaction;

#endif