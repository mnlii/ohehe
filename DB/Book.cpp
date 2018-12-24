//
// Created by tahara on 18-12-20.
//

#include "Book.h"


char kk[20];
void intTostr(int num){
    memset(kk, 0, sizeof(kk));
    int i, tmp;
    tmp = num;
    for(i = 10; i >= 0; i--){
        kk[i] = static_cast<char>(tmp % 10 + '0');
        tmp = tmp / 10;
    }
}


Book::Book() {
    bkdb = new BLL<char[20], data>(dbname);
    bkdb -> indexFlg = true;
    r_author = new Refer(db_r_auth);
    r_keyword = new Refer(db_r_keyword);
    r_name = new Refer(db_r_name);
    r_ISBN = new Refer(db_r_ISBN);
    bkdb -> setFunction(reinterpret_cast<bool (*)(const char (&)[20], const char (&)[20])>(cmp),
                        reinterpret_cast<bool (*)(const char (&)[20], const char (&)[20])>(equal));
}


Book::~Book() {
    delete bkdb;
}

void Book::upref(){
    int j;
    if(!bkdb -> old_t.empty()){
        int len = static_cast<int>(bkdb -> old_t.size()), i;
        for(i = 0; i < len; i++){
            r_ISBN -> rmreference(bkdb -> dlist[i].ISBN, bkdb -> old_t[i]);
            r_ISBN -> addreference(bkdb -> dlist[i].ISBN, bkdb -> new_t[i]);
            r_author -> rmreference(bkdb -> dlist[i].author, bkdb -> old_t[i]);
            r_author -> addreference(bkdb -> dlist[i].author, bkdb -> new_t[i]);
            r_name -> rmreference(bkdb -> dlist[i].name, bkdb -> old_t[i]);
            r_name -> addreference(bkdb -> dlist[i].name, bkdb -> new_t[i]);

            j = 0;
            char tmp[100];
            for(int k = 0; bkdb -> dlist[i].keyword[k]; k++){
                if(bkdb -> dlist[i].keyword[k] == '|'){
                    tmp[j] = 0;
                    r_keyword -> rmreference(tmp, bkdb -> old_t[i]);
                    r_keyword -> addreference(tmp, bkdb -> new_t[i]);
                    j = 0;
                }
                else{
                    tmp[j++] = bkdb -> dlist[i].keyword[k];
                }
            }
            tmp[j] = 0;
            r_keyword -> rmreference(tmp, bkdb -> old_t[i]);
            r_keyword -> addreference(tmp, bkdb -> new_t[i]);
        }
    }
}



bool Book::addBook(data& ins) {
    intTostr(bkdb -> getDTN());
    off_t offt;
    strcpy(ins.pk, kk);

    offt = bkdb->insert(reinterpret_cast<const char (&)[20]>(*kk), ins);


    r_ISBN -> addreference(ins.ISBN, offt);
    r_name -> addreference(ins.name, offt);
    r_author -> addreference(ins.author, offt);
    char tmpkey[40];
    int j = 0;
    for(int i = 0; ins.keyword[i]; i++){
        if(ins.keyword[i] == '|'){
            tmpkey[j] = 0;
            r_keyword -> addreference(tmpkey, offt);
            j = 0;
        }
        else{
            tmpkey[j++] = ins.keyword[i];
        }
    }
    tmpkey[j] = 0;
    r_keyword -> addreference(tmpkey, offt);

    upref();
    return true;
}

/*
 * not finished now
 *
 */
bool Book::modify(char *ISBN, char *name, char *author, char *keyword, double price, char* oldKey) {
    data ins, rm;
    vector<data> qret;
    int i, j;
    char tmp[40];

    qret = bkdb -> query((const char (&)[20])(*oldKey));

    if(qret.empty()){
        return false;
    }

    rm = qret[0];
    r_ISBN -> rmreference(rm.ISBN, bkdb -> lst_query);
    r_name ->rmreference(rm.name, bkdb -> lst_query);
    r_author -> rmreference(rm.author, bkdb -> lst_query);
    j = 0;
    for(i = 0; rm.keyword[i]; i++){
        if(rm.keyword[i] == '|'){
            tmp[j] = 0;
            r_keyword -> rmreference(tmp, bkdb -> lst_query);
            j = 0;
        }
        else{
            tmp[j++] = rm.keyword[i];
        }
    }
    tmp[j] = 0;
    r_keyword -> rmreference(tmp, bkdb -> lst_query);

    strcpy(ins.pk, rm.pk);
    strcpy(ins.ISBN, ISBN);
    strcpy(ins.name, name);
    strcpy(ins.author, author);
    strcpy(ins.keyword, keyword);
    ins.price = price;
    ins.store = rm.store;


    off_t offt = bkdb -> update(reinterpret_cast<const char (&)[20]>(*oldKey), ins);

    r_ISBN -> addreference(ISBN, bkdb -> lst_query);
    r_name -> addreference(name, bkdb -> lst_query);
    r_author -> addreference(author, bkdb -> lst_query);
    j = 0;
    for(i = 0; keyword[i]; i++){
        if(keyword[i] == '|'){
            tmp[j] = 0;
            r_keyword -> addreference(tmp, bkdb -> lst_query);
            j = 0;
        }
        else{
            tmp[j++] = keyword[i];
        }
    }
    tmp[j] = 0;
    r_keyword -> addreference(tmp, bkdb -> lst_query);
    upref();
    return true;
}

bool Book::sell_buy(const char *ISBN, int num) {
    vector<off_t> qres = r_ISBN -> getref(const_cast<char *>(ISBN));
    data now = bkdb -> getdata(qres[0]);

    if(now.store + num < 0) {
        return false;
    }
    now.store += num;
    bkdb -> update(reinterpret_cast<const char (&)[20]>(now.pk), now);
    return true;
}

Book::data Book::query(const char *ISBN) {
    vector<off_t> qres;
    qres = r_ISBN -> getref(const_cast<char *>(ISBN));
    return bkdb -> getdata(qres[0]);
}

bool Book::exist(char *ISBN) {
    vector<off_t> qres = r_ISBN -> getref(ISBN);
    return qres.size() == 1;
}

vector<Book::data> Book::get_all() {
    vector<data> res = bkdb -> getall();
    return res;
}

vector<Book::data> Book::get_name(char *name) {
    vector<off_t> qret = r_name -> getref(name);
    vector<data> res;
    int i, len = qret.size();
    for(i = 0; i < len; i++){
        res.push_back(bkdb -> getdata(qret[i]));
    }
    return res;
}

vector<Book::data> Book::get_keyword(char *keyword) {
    vector<off_t> qret = r_keyword-> getref(keyword);
    vector<data> res;
    int i, len = qret.size();
    for(i = 0; i < len; i++){
        res.push_back(bkdb -> getdata(qret[i]));
    }
    return res;
}

vector<Book::data> Book::get_author(char *author) {
    vector<off_t> qret = r_author -> getref(author);
    vector<data> res;
    int i, len = qret.size();
    for(i = 0; i < len; i++){
        res.push_back(bkdb -> getdata(qret[i]));
    }
    return res;
}

vector<Book::data> Book::get_ISBN(char *ISBN) {
    vector<off_t> qret = r_ISBN -> getref(ISBN);
    vector<data> res;
    int i, len = qret.size();
    for(i = 0; i < len; i++){
        res.push_back(bkdb -> getdata(qret[i]));
    }
    return res;
}
