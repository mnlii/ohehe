//
// Created by tahara on 18-12-22.
//

#ifndef BOOKSTORE_COMMANDREADER_H
#define BOOKSTORE_COMMANDREADER_H

#include "DB/User.h"
#include "Controller/UserController.h"
#include "Controller/BookController.h"
#include "Reporter/Command.h"
#include "Reporter/Finance.h"


class CommandReader {
private:
    User::data usr;
    bool isLogin;
    bool selected;

    UserController uc;
    BookController bc;

    Command* Global;
    Command* Ruser;
    Finance* Frecorder;

    char* argvs[20];
    /*
     * user commands
     */

    int login(int argc, char* argv[]);
    bool logout(int argc, char* argv[]);
    bool useradd(int argc, char* argv[]);
    bool Userregister (int argc, char* argv[]);
    bool Userdelete(int argc, char* argv[]);
    bool passwd(int argc, char* argv[]);
    /*
     * book recoder.
     */

    bool select(int argc, char* argv[]);
    bool modify(int argc, char* argv[]);
    bool import(int argc, char* argv[]);
    bool show(int argc, char* argv[]);
    bool buy(int argc, char* argv[]);
    bool show_finance(int argc, char* argv[]);

    /*
     * command processor;
     */

    bool CommandProcessor(int argc, char* argv[]);

    /*
     * Another method
     */
    //bool GetSuper();

public:
    bool GetSuper();
    CommandReader();
    ~CommandReader();
    void NextCommand(char* cmd);
};


#endif //BOOKSTORE_COMMANDREADER_H
