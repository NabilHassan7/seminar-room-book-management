#ifndef FILE_H
#define FILE_H

void initializeSystemFiles(void);
void ensureDirectoryExists(const char *directoryPath);
void ensureBinaryFileExists(const char *filePath);

#endif