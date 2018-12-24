//
// Created by tahara on 18-12-22.
//

#include "BookController.h"


bool cmp(string a, string b){
    int lena = static_cast<int>(a.length());
    int lenb = static_cast<int>(b.length());
    int i;
    for(i = 0; i < lena && i < lenb; i++){
        if(a[i] != b[i]) return a[i] < b[i];
    }
    if(i == lena) return false;
    else return true;
}


double BookController::strtd(const char* str){
    int flg = 1;
    double res = 0, tmp;
    int i = 0;
    if(str[i] == '-'){
        i++;
        flg = -1;
    }
    for(; str[i] && str[i] != '.'; i++){
        res = res * 10 + str[i] - '0';
    }

    if(str[i] && str[i] == '.'){
        i++;
        tmp = 10;
        for(; str[i]; i++){
            res = res + (str[i] - '0') / tmp;
            tmp = tmp * 10;
        }
    }
    res = res * flg;

    return res;
}

BookController::BookController() {
    dt.init();
}

void BookController::select(char *ISBN) {
    if(bkdb.exist(ISBN)){
        dt = bkdb.query(ISBN);
    }
    else {
        dt.init();
        strcpy(dt.ISBN, ISBN);
        bkdb.addBook(dt);
    }
}

bool BookController::modify(int argc, char * argv[]) {
    int i = 0, j;
    char key[40];
    char value[40];
    int tk, tv;

    for(i = 0; i < argc; i++){
        tk = tv = 0;
        if(argv[i][0] != '-') return false;
        for(j = 1; argv[i][j] != '='; j++){
            key[tk++] = argv[i][j];
        }
        j++;
        for(; argv[i][j]; j++){
            value[tv++] = argv[i][j];
        }
        value[tv] = key[tk] = 0;
        if(strcmp(key, "ISBN") == 0) {
            if(strcmp(dt.ISBN, value) != 0 && bkdb.exist(value)) return false;
            strcpy(dt.ISBN, value);
        }
        else if(strcmp(key, "name") == 0) {
            strcpy(dt.name, value + 1);
            dt.name[strlen(dt.name) - 1] = 0;
        }
        else if(strcmp(key, "author") == 0) {
            strcpy(dt.author, value + 1);
            dt.author[strlen(dt.author) - 1] = 0;
        }
        else if(strcmp(key, "keyword") == 0) {
            strcpy(dt.keyword, value + 1);
            dt.keyword[strlen(dt.keyword) - 1] = 0;
        }
        else if(strcmp(key, "price") == 0){
            dt.price = strtd(value);
        }
        else{
            return false;
        }
    }
    bkdb.modify(dt.ISBN, dt.name, dt.author, dt.keyword, dt.price, dt.pk);
    return true;
}

bool BookController::sell_buy(const char* ISBN, int num) {
    if(num < 0 && !bkdb.query(ISBN).full_data()) return false;
    return bkdb.sell_buy(ISBN, num);
}

string conv(Book::data dt){
    char tmp[20];
    string res;
    res = res + dt.ISBN + "\t" + dt.name + "\t" + dt.author + "\t" + dt.keyword + "\t";
    sprintf(tmp, "%.2lf", dt.price);
    res = res + tmp + "\t";
    sprintf(tmp, "%d本", dt.store);
    res = res + tmp;
    return res;
}

vector<string> BookController::show(int argc, char* argv[], bool& flg) {
    flg = true;
    vector<Book::data> qres;
    vector<string> res;
    string pdata;
    int i, len;

    if(argc == 0){
        qres = bkdb.get_all();
        len = static_cast<int>(qres.size());
        for(i = 0; i < len; i++){
            if(!qres[i].full_data()){
                flg = false;
            }
            res.push_back(conv(qres[i]));
        }
        std::sort(res.begin(), res.end(), cmp);
        return res;
    }
    char key[50], value[50];
    int kt, vt;
    i = kt = vt = 0;

    if(argv[0][i] == '-') {
        i++;
    }
    while(argv[0][i] != '='){
        key[kt++] = argv[0][i];
        i++;
    }
    i++;
    while(argv[0][i]){
        value[vt++] = argv[0][i];
        i++;
    }
    key[kt] = value[vt] = 0;
    if(strcmp(key, "ISBN") == 0){
        qres = bkdb.get_ISBN(value);
    }
    else if(strcmp(key, "name") == 0){
        value[strlen(value) - 1] = 0;
        qres = bkdb.get_name(value + 1);
    }
    else if(strcmp(key, "author") == 0){
        value[strlen(value) - 1] = 0;
        qres = bkdb.get_author(value + 1);
    }
    else if(strcmp(key, "keyword") == 0){
        value[strlen(value) - 1] = 0;
        qres = bkdb.get_keyword(value + 1);
    }
    else{
        flg = false;
        return res;
    }
    len = static_cast<int>(qres.size());
    for(i = 0; i < len; i++){
        if(!qres[i].full_data()){
            flg = false;
        }
        res.push_back(conv(qres[i]));
    }

    std::sort(res.begin(), res.end(), cmp);
    return res;
}

const char *BookController::getISBN() {
    return dt.ISBN;
}

double BookController::get_price(const char *ISBN) {
    dt = bkdb.query(ISBN);
    return dt.price;
}
