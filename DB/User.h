//
// Created by tahara on 18-12-15.
//

#ifndef BOOKSTORE_USER_H
#define BOOKSTORE_USER_H

#include <vector>

#include "BLL.h"

#define KT char[USERLEN]
#define DT data

#define USERLEN 50
#define PWDLEN 50

using std::vector;


/*
 *  Set User database.
 *  key:    username char[50]
 *  data:   username char[50]
 *          password char[50]
 *          permission int{7 3 1 0}
 *
 *  we use username to be the key of the database.
 *  database's location is ./customer.db
 *
 */
class User {
public:
    struct data {
        char user[USERLEN];
        char name[USERLEN];
        char password[PWDLEN];
        int permission;
        data(){
            memset(name, 0, sizeof(name));
            memset(user, 0, sizeof(user));
            memset(password, 0, sizeof(password));
            permission = 0;
        }
        bool operator==(const data& b){
            return (strcmp(user, b.user) == 0 && strcmp(password, b.password) == 0);
        }
    };
private:
    const char *dbname = "User.db";

    BLL<KT, DT> *cusdb;

    bool static cmp(const char a[USERLEN], const char b[USERLEN]) {
        return strcmp(a, b) < 0;
    }

    bool static equal(const char a[USERLEN], const char b[USERLEN]) {
        return strcmp(a, b) == 0;
    }

public:
    User();
    bool addUser(const char user[USERLEN], const char pwd[PWDLEN], const char name[USERLEN], const int permission);
    int logIn(const char user[USERLEN], const char pwd[PWDLEN]);

    int getPermission(const char user[USERLEN]);

    bool removeUser(const char[USERLEN]);
    bool updateUser(const char[USERLEN], const char pwd[PWDLEN]);
    bool hasUser(const char user[USERLEN]);
    ~User();
};

#endif //BOOKSTORE_CUSTOMERS_H
