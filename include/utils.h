#ifndef UTILS_H
#define UTILS_H

void clearInputBuffer(void);
void readLine(char *buffer, int size);
int readInt(const char *prompt);
int readPositiveInt(const char *prompt);
void printDivider(int length);
void pauseScreen(void);

#endif