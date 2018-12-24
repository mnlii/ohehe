//
// Created by tahara on 18-12-21.
//

#ifndef BOOKSTORE_FINANCE_H
#define BOOKSTORE_FINANCE_H

#include "Reporter.h"

class Finance {
private:
    const char* path = "FinanceRecord";
    struct piece{
        char ISBN[20];
        char name[50];
        double price;
        int quantity;
    };
    Reporter<piece>* rp;
public:
    Finance();
    void insert(const char ISBN[20], const char name[60], double price, int quantity);
    vector<piece> query(int num = -1);
    vector<double> qfinance(int num = -1);
};


#endif //BOOKSTORE_FINANCE_H
