#ifndef DATE_H
#define DATE_H

#include "models.h"

Date getCurrentDate(void);
Date addDays(Date date, int days);
int isEmptyDate(Date date);
void printDate(Date date);

#endif