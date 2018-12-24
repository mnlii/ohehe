//
// Created by tahara on 18-12-21.
//

#ifndef BOOKSTORE_REFER_H
#define BOOKSTORE_REFER_H


/*
 * Reference to database.
 * while making this system I observed all reference's datatype
 * is "char*" , so I simply make this class's key type is "char*"
 * When I have enough time, I'll rewrite this part codes by <tmplate>
 *
 * key: the key for this reference.
 * DB file path: the database path of db system.
 * data: offset of the data of key x
 */

#include <cstdio>
#include <vector>
#include <cstdio>
#include "BLL.h"
#define STRLEN 80

using std::vector;

class Refer {
private:
    // define data struct of reference database.
    char rkey[STRLEN];
    struct node{
        char rkey[STRLEN];
        off_t offt;
        bool operator==(const node& b){
            return offt == b.offt && strcmp(rkey, b.rkey) == 0;
        }
    };
    char dbpath[50];
    BLL<char[STRLEN], node>* dblnk;

    bool static cmp(const char a[STRLEN], const char b[STRLEN]) {
        return strcmp(a, b) < 0;
    }

    bool static equal(const char a[STRLEN], const char b[STRLEN]) {
        return strcmp(a, b) == 0;
    }

public:
    Refer(const char dbpath[]);
    bool rmreference(char key[STRLEN], off_t rft);
    bool addreference(char key[STRLEN], off_t rft);
    vector<off_t> getref(char key[STRLEN]);
};

#endif //BOOKSTORE_REFER_H
