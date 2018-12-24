//
// Created by tahara on 18-12-15.
//

#include "User.h"
#include "BLL.h"


User::User() {
    cusdb = new BLL<char[USERLEN], data>(dbname);
    cusdb -> setFunction(reinterpret_cast<bool (*)(const char (&)[50], const char (&)[50])>(cmp),
                         reinterpret_cast<bool (*)(const char (&)[50], const char (&)[50])>(equal));
}

User::~User() {
    delete cusdb;
}

bool User::addUser(const char *user, const char *pwd, const char* name, const int permission) {
    vector<data> q;
    q = cusdb -> query((const char (&)[50])(*user));
    if(!q.empty()){
        return false;
    }
    data ins;
    strcpy(ins.name, name);
    strcpy(ins.user, user);
    strcpy(ins.password, pwd);
    ins.permission = permission;
    cusdb -> insert((const char(&)[50]) (*user), ins);
    return true;
}

int User::logIn(const char *user, const char *pwd) {
    vector<data> res;
    res = cusdb -> query((const char (&)[50])(*user));
    if(res.size() != 1 || strcmp(res[0].password, pwd) != 0){
        return 0;
    }
    return res[0].permission;
}

bool User::hasUser(const char *user) {
    vector<data> res;
    res = cusdb -> query((const char (&)[50])(*user));
    return res.size() == 1;
}

bool User::removeUser(const char *user) {
    vector<data> res;
    res = cusdb -> query((const char(&)[50])(*user));
    if(res.size() != 1) return false;
    cusdb -> removedata((const char (&)[50]) (*user), res[0]);
    return true;
}

bool User::updateUser(const char *user, const char* pwd) {
    vector<data> res;
    res = cusdb -> query((const char(&)[50])(*user));
    if(res.size() != 1) return false;

    strcpy(res[0].password, pwd);

    cusdb -> update((const char (&)[50]) (*user), res[0]);
    return true;
}

int User::getPermission(const char *user) {
    vector<data> res;
    res = cusdb -> query((const char(&)[50])(*user));
    if(res.size() != 1) return 0;
    return res[0].permission;
}



