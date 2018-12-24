//
// Created by tahara on 18-12-22.
//

#include "UserController.h"

int UserController::login(const char *username, const char *pwd) {
    return usr.logIn(username, pwd);
}

bool UserController::useradd(const char *username, const char *pwd, const char* name, const int& permission) {
    return usr.addUser(username, pwd, name, permission);
}

bool UserController::deleteUser(const char *user) {
    return usr.removeUser(user);
}

bool UserController::changepwd(const char *username, const char *pwd) {
    return usr.updateUser(username, pwd);
}

int UserController::getPermission(const char *username) {
    return usr.getPermission(username);
}



