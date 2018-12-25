#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "basic_function.h"
#include "CommandReader.h"

#define TESTROOT

#ifdef TESTROOT
#include "DB/User.h"
#endif

void load_command(const char* fpath){
    FILE* fptr = fopen(fpath, "r");
    CommandReader cmd;
    cmd.NextCommand("su root sjtu");

    char ord[201];
    while(fgets(ord, 200, fptr)){
        ord[strlen(ord) - 1] = 0;
//        printf("ORD: %s\n", ord);
        if(strcmp(ord, "exit") == 0){
            fclose(fptr);
            return;
        }
        cmd.NextCommand(ord);
    }
    fclose(fptr);
}

int main()
{

#ifdef TESTROOT
    User usr;
    usr.addUser("root", "sjtu", "name", 7);
#endif


    if(exist_file("command.txt")){
        load_command("command.txt");
        return 0;
    }


    CommandReader cmd;
    char ord[101];
    while(true){
        fgets(ord, 100, stdin);
        ord[strlen(ord) - 1] = 0;
        if(strcmp(ord, "exit") == 0){
            break;
        }
        cmd.NextCommand(ord);
    }
    return 0;
}
