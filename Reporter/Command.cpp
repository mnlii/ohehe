//
// Created by tahara on 18-12-21.
//

#include <cstring>
#include "Command.h"

Command::Command(const char *pth) {
    strcpy(path, pth);
    rp = new Reporter<cmd>(path);
}

void Command::insert(char *user, char *msg) {
    cmd ins;
    strcpy(ins.user, user);
    strcpy(ins.msg, msg);
    rp -> add_record(ins);

}

vector<Command::cmd> Command::get() {
    return rp ->get_record();
}
