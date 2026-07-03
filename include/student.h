#ifndef STUDENT_H
#define STUDENT_H

void registerStudent(void);
void removeStudent(void);
void viewRegisteredStudents(void);
void searchStudent(void);

int studentExistsById(const char *studentId);

#endif