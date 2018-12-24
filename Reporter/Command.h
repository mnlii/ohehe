//
// Created by tahara on 18-12-21.
//

#ifndef BOOKSTORE_COMMAND_H
#define BOOKSTORE_COMMAND_H

#include "Reporter.h"


/*
 *  Sorry for my bad name for Command Reporter.
 *  this class is about collect command for users.
 */
class Command {
private:
    char path[50];
    struct cmd{
        char user[20];
        char msg[100];
    };
    Reporter<cmd> * rp;

public:
    Command(const char* pth);
    void insert(char user[20], char msg[100]);
    vector<cmd> get();

};


#endif //BOOKSTORE_COMMAND_H
