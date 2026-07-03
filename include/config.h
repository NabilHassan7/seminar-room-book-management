#ifndef CONFIG_H
#define CONFIG_H

#define APP_NAME "EEE Seminar Room Book Management System"
#define DEPARTMENT_NAME "Department of Electrical and Electronic Engineering"
#define UNIVERSITY_NAME "University of Dhaka"

/* File Paths */
#define BOOKS_FILE "data/books.dat"
#define USERS_FILE "data/users.dat"
#define TRANSACTIONS_FILE "data/transactions.dat"

/* Report Paths */
#define REPORTS_DIR "reports/"
#define EXPORTS_DIR "exports/"

/* System Limits */
#define MAX_ID_LEN 20
#define MAX_NAME_LEN 100
#define MAX_TITLE_LEN 150
#define MAX_AUTHOR_LEN 100
#define MAX_CATEGORY_LEN 80
#define MAX_COMPANY_LEN 100
#define MAX_EDITION_LEN 50
#define MAX_DEPARTMENT_LEN 100
#define MAX_PASSWORD_LEN 50
#define MAX_ROLE_LEN 20
#define MAX_STATUS_LEN 20

/* Rules */
#define LOAN_PERIOD_DAYS 14
#define FINE_PER_DAY 10.0

/* Admin Account */
#define DEFAULT_ADMIN_ID "admin"
#define DEFAULT_ADMIN_PASSWORD "admin123"
#define DEFAULT_ADMIN_NAME "System Administrator"
#define DEFAULT_ADMIN_ROLE "ADMIN"
#define DEFAULT_ADMIN_DEPARTMENT "EEE"

/* Roles */
#define ROLE_ADMIN "ADMIN"
#define ROLE_STUDENT "STUDENT"

/* Status Values */
#define STATUS_ACTIVE 1
#define STATUS_INACTIVE 0

#define TRANSACTION_ISSUED "ISSUED"
#define TRANSACTION_RETURNED "RETURNED"

#define FINE_PAID 1
#define FINE_UNPAID 0

#endif