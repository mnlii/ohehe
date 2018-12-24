//
// Created by tahara on 18-12-21.
//

#include "Refer.h"
#include "Book.h"
#include "../Controller/BookController.h"

#include <cstdio>

Refer::Refer(const char *dbpath) {
    strcpy(this->dbpath, dbpath);
    dblnk = new BLL<char[STRLEN], node>(dbpath);
    dblnk -> setFunction(reinterpret_cast<bool (*)(const char (&)[80], const char (&)[80])>(cmp),
                         reinterpret_cast<bool (*)(const char (&)[80], const char (&)[80])>(equal));
}

bool Refer::rmreference(char *key, off_t rft) {
    node del;
    del.offt = rft;
    strcpy(del.rkey, key);
    dblnk -> removedata(reinterpret_cast<const char (&)[80]>(*key), del);
    return true;
}

bool Refer::addreference(char *key, off_t rft) {
    node ins;
    ins.offt = rft;
    strcpy(ins.rkey, key);
    dblnk -> insert(reinterpret_cast<const char (&)[80]>(*key), ins);
    return true;
}

vector<off_t> Refer::getref(char *key) {
    vector<off_t> res;
    vector<node> qret;
    qret = dblnk -> query((const char (&)[80])(*key));
    for(int i = 0; i < qret.size(); i++){
        res.push_back(qret[i].offt);
    }
    return res;
}
