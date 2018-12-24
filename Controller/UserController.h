//
// Created by tahara on 18-12-22.
//

#ifndef BOOKSTORE_USERCONTROLLER_H
#define BOOKSTORE_USERCONTROLLER_H

#include "../DB/User.h"

class UserController {
private:
    User usr;
public:
    int login(const char* username, const char* pwd);
    int getPermission(const char* username);
    bool useradd(const char* username, const char* pwd, const char* name, const int& permission);
    bool deleteUser(const char* user);
    bool changepwd(const char* username,const char* pwd);
};

#endif //BOOKSTORE_USERCONTROLLER_H
