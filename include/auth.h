#ifndef AUTH_H
#define AUTH_H

#include "models.h"

void seedAdminAccount(void);
int adminExists(void);
int login(User *loggedInUser);

#endif