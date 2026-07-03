#ifndef TRANSACTION_H
#define TRANSACTION_H

void issueBook(void);
void returnBook(void);

int getNextTransactionId(void);
int hasActiveIssue(const char *studentId, const char *bookId);

#endif