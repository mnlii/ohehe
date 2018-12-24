//
// Created by tahara on 18-12-22.
//

#include "CommandReader.h"
#include <cstdio>
CommandReader::CommandReader() {
    isLogin = false;
    selected = false;
    Ruser = nullptr;

    Global = new Command("Global");
    Frecorder = new Finance();
    for(int i = 0; i < 20; i++)
        argvs[i] = new char[80];
}

CommandReader::~CommandReader() {
    delete Global;
    delete Frecorder;
    for(int i = 0; i < 20; i++)
        argvs[i] = new char[20];
    if(Ruser != nullptr){
        delete Ruser;
    }
}

void CommandReader::NextCommand(char* cmd) {
    int argc = 0;
    int i = 0, j;
    bool res;


    while(cmd[i]){
        if(cmd[i] != ' ') {
            j = 0;
            int cnt;
            cnt = 0;
            while (cnt == 1 || (cmd[i] && cmd[i] != ' ')){
                if(cmd[i] == '\"') {
                    if(cnt == 0)
                        cnt = 1;
                    else
                        cnt = 0;
                }
                argvs[argc][j++] = cmd[i++];
            }
            argvs[argc][j] = 0;
            argc++;
        }
        else
            i++;
    }

    if(strcmp(argvs[0], "su") == 0){
        res = static_cast<bool>(login(argc - 1, argvs + 1));
    }
    else if(strcmp(argvs[0], "logout") == 0){
        res = logout(argc - 1, argvs + 1);
    }
    else if(strcmp(argvs[0], "useradd") == 0){
        res = useradd(argc - 1, argvs + 1);
    }
    else if(strcmp(argvs[0], "register") == 0){
        res = Userregister(argc - 1, argvs + 1);
    }
    else if(strcmp(argvs[0], "delete") == 0){
        res = Userdelete(argc - 1, argvs + 1);
    }
    else if(strcmp(argvs[0], "passwd") == 0){
        res = passwd(argc - 1, argvs + 1);
    }
    else if(strcmp(argvs[0], "select") == 0){
        res = select(argc - 1, argvs + 1);
    }
    else if(strcmp(argvs[0], "modify") == 0){
        res = modify(argc - 1, argvs + 1);
    }
    else if(strcmp(argvs[0], "show") == 0){
        res = show(argc - 1, argvs + 1);
    }
    else if(strcmp(argvs[0], "buy") == 0){
        res = buy(argc - 1, argvs + 1);
    }
    else if(strcmp(argvs[0], "import") == 0){
        res = import(argc - 1, argvs + 1);
    }
    else if(strcmp(argvs[0], "load") == 0){
        res =  CommandProcessor(argc - 1, argvs + 1);
    }
    else{
        res = false;
    }
    if(!res) printf("Invalid\n");
    /*
    if(res) printf("Yes!!!\n");
    else printf("\n");
     */
}

int CommandReader::login(int argc, char* argv[]) {
    if(isLogin && argc == 1){
        int permission = uc.getPermission(argv[0]);
        if(uc.getPermission(usr.user) > permission){
            strcpy(usr.user, argv[0]);
            usr.permission = permission;
            return true;
        }
        else{
            return false;
        }
    }

    if(argc != 2) return false;
//    if(argc != 2 || isLogin) return false;

    int permission = uc.login(argv[0], argv[1]);
    if(permission){
        usr.permission = permission;
        strcpy(usr.user, argv[0]);
        strcpy(usr.password, argv[1]);
        isLogin = true;
        selected = false;
        return true;
    }
    else{
        return false;
    }
}

bool CommandReader::logout(int argc, char **argv) {
    if(argc != 0 || !isLogin) return false;

    isLogin = false;
    selected = false;

    usr.permission = 0;
    delete Ruser;
    Ruser = nullptr;

    return true;
}

bool CommandReader::useradd(int argc, char **argv) {
    if((usr.permission & 2) != 2) return false;
    if(argc != 4) return false;
    User::data ins;
    strcpy(ins.user, argv[0]);
    strcpy(ins.password, argv[1]);
    ins.permission = argv[2][0] - '0';
    if(ins.permission >= usr.permission) return false;
    strcpy(ins.name, argv[3]);
    return uc.useradd(ins.user, ins.password, ins.name, ins.permission);
}

bool CommandReader::Userregister(int argc, char **argv) {
    if(argc != 3) return false;
    User::data ins;
    strcpy(ins.user, argv[0]);
    strcpy(ins.password, argv[1]);
    strcpy(ins.name, argv[2]);
    ins.permission = 1;
    return uc.useradd(ins.user, ins.password, ins.name, ins.permission);
}

bool CommandReader::Userdelete(int argc, char **argv) {
    if(usr.permission != 7) return false;
    if(argc != 1) return false;
    return uc.deleteUser(argv[0]);
}

bool CommandReader::passwd(int argc, char **argv) {
    if(usr.permission == 7 && argc == 2){
        return uc.changepwd(argv[0], argv[1]);
    }
    else{
        if(argc != 3) return false;
        if(!uc.login(argv[0], argv[1])) return false;
        return uc.changepwd(argv[0], argv[2]);
    }
}

bool CommandReader::select(int argc, char **argv) {
    if(argc != 1) return false;
    bc.select(argv[0]);
    selected = true;
    return true;
}

bool CommandReader::modify(int argc, char **argv) {
    if(argc == 0 || argc > 5) return false;
    if(!selected) return false;
    return bc.modify(argc, argv);
}

bool CommandReader::import(int argc, char **argv) {
    if(usr.permission < 3) return false;
    if(argc != 2) return false;
    if(!selected) return false;
    Frecorder -> insert(bc.getISBN(), usr.name, -bc.strtd(argv[1]), atoi(argv[0]));
    bc.sell_buy(bc.getISBN(), atoi(argv[0]));
    return true;
}

bool CommandReader::show(int argc, char **argv) {
    if(usr.permission < 1) return false;
    if(argc == 0){
        bool flg;
        int i;
        vector<string> qres = bc.show(argc, argv, flg);
        sort(qres.begin(),qres.end());
        qres.erase(unique(qres.begin(), qres.end()), qres.end());

        for(i = 0; i < qres.size(); i++){
            printf("%s\n", qres[i].c_str());
        }
        return true;
    }

    if(strcmp(argv[0], "finance") == 0){
        return show_finance(argc - 1, argv + 1);
    }

    bool flg;
    int i;
    vector<string> qres = bc.show(argc, argv, flg);
    sort(qres.begin(),qres.end());
    qres.erase(unique(qres.begin(), qres.end()), qres.end());
    if(!flg) return false;

    sort(qres.begin(), qres.end());

    for(i = 0; i < qres.size(); i++){
        printf("%s\n", qres[i].c_str());
    }
    return true;
}

bool CommandReader::buy(int argc, char **argv) {
    if(argc != 2) return false;
    double price = bc.get_price(argv[0]);
    int flg = bc.sell_buy(argv[0], -atoi(argv[1]));
    if(!flg) return false;
    Frecorder -> insert(argv[0], usr.name, price * atoi(argv[1]), atoi(argv[1]));
    return true;
}

bool CommandReader::show_finance(int argc, char *argv[]) {
    vector<double> qres = Frecorder -> qfinance();
    double r1, r2;
    int i, len;
    r1 = r2 = 0;
    if(argc == 0){
        for(i = 0; i < qres.size(); i++){
            if(qres[i] > 0)
                r1 += qres[i];
            else
                r2 += (-qres[i]);
        }
        printf("+ %.2lf - %.2lf\n", r1, r2);
        return true;
    }
    if(argc > 1) return false;
    len = static_cast<int>(qres.size());
    for(i = len - atoi(argv[0]); i < len; i++){
        if(qres[i] > 0)
            r1 += qres[i];
        else
            r2 += (-qres[i]);
    }
    printf("+ %.2lf - %.2lf\n", r1, r2);
    return true;
}

bool CommandReader::CommandProcessor(int argc, char *argv[]) {
    if (usr.permission != 7) return false;
    if(argc != 1) return false;
    FILE* fptr = fopen(argv[0], "r");
    char order[101];
    if(!fptr){
        return false;
    }
    CommandReader cmd;
    cmd.GetSuper();
    while(fgets(order, 101, fptr)) {
        order[strlen(order) - 1] = 0;
        if (strcmp(order, "exit") == 0) {
            fclose(fptr);
            return true;
        }
        cmd.NextCommand(order);
    }
    fclose(fptr);
    return true;
}

bool CommandReader::GetSuper() {
    usr.permission = 7;
    return true;
}
