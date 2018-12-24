//
// Created by tahara on 18-12-21.
//

#include <cstring>
#include "Finance.h"

Finance::Finance() {
    rp = new Reporter<piece>(path);
}

void Finance::insert(const char *ISBN, const char *name, double price, int quantity) {
    piece ins;
    strcpy(ins.ISBN, ISBN);
    strcpy(ins.name, name);
    ins.price = price;
    ins.quantity = quantity;
    rp -> add_record(ins);
}

vector<Finance::piece> Finance::query(int num) {
    vector<piece> qres = rp -> get_record();
    if(num == -1)
        num = static_cast<int>(qres.size());

    vector<piece> res;
    int len, i;
    len = static_cast<int>(qres.size());
    for(i = len - num; i < len; i++){
        res.push_back(qres[i]);
    }

    return res;
}

vector<double> Finance::qfinance(int num) {
    vector<piece> qres = query(num);
    vector<double> db;
    int i;

    for(i = 0; i < qres.size(); i++){
        db.push_back(qres[i].price);
    }
    return db;
}
