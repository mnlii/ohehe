//
// Created by tahara on 18-12-22.
//

#ifndef BOOKSTORE_BOOKCONTROLLER_H
#define BOOKSTORE_BOOKCONTROLLER_H

#include "../DB/Book.h"
#include <iostream>


using std::string;

class BookController {
private:
    Book bkdb;
    Book::data dt;
public:
    BookController();
    double strtd(const char* str);
    void select(char ISBN[20]);
    bool modify(int argc, char* argv[]);
    bool sell_buy(const char* ISBN, int num);
    double get_price(const char* ISBN);
    const char* getISBN();

    vector<string> show(int argc, char* argv[], bool& flg);
};

#endif //BOOKSTORE_BOOKCONTROLLER_H
