#ifndef REPORT_H
#define REPORT_H

void reportsMenu(void);

void generateAvailableBooksReport(void);
void generateIssuedBooksReport(void);
void generateOverdueBooksReport(void);
void generateCategoryWiseReport(void);
void generateIssueHistoryReport(void);
void generateUnpaidFineReport(void);

#endif