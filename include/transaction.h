#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "models.h"

void issueBook(void);
void returnBook(void);

int getNextTransactionId(void);
int hasActiveIssue(const char *studentId, const char *bookId);
int hasUnpaidFine(const char *studentId);

int calculateOverdueDays(Date dueDate, Date checkDate);
double calculateFineAmount(int overdueDays);

#endif