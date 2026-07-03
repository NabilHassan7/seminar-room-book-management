#include <stdio.h>
#include <time.h>

#include "../../include/config.h"
#include "../../include/models.h"
#include "../../include/date.h"

Date getCurrentDate(void)
{
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    Date date;
    date.day = local->tm_mday;
    date.month = local->tm_mon + 1;
    date.year = local->tm_year + 1900;

    return date;
}

Date addDays(Date date, int days)
{
    struct tm temp = {0};

    temp.tm_mday = date.day + days;
    temp.tm_mon = date.month - 1;
    temp.tm_year = date.year - 1900;

    mktime(&temp);

    Date result;
    result.day = temp.tm_mday;
    result.month = temp.tm_mon + 1;
    result.year = temp.tm_year + 1900;

    return result;
}

int isEmptyDate(Date date)
{
    return date.day == 0 && date.month == 0 && date.year == 0;
}

void printDate(Date date)
{
    if (isEmptyDate(date))
    {
        printf("N/A");
    }
    else
    {
        printf("%02d-%02d-%04d", date.day, date.month, date.year);
    }
}