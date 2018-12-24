//
// Created by tahara on 18-12-20.
//

#ifndef BOOKSTORE_BOOK_H
#define BOOKSTORE_BOOK_H

#include "BLL.h"
#include "Refer.h"

#define STRLEN 80

/* Database for books
 * key:  ISBN  char[20]
 * data:    ISBN  char[20]
 *          name  char[20]
 *          author char[20]
 *          keyword char[20]
 *          quantity int
 *
 */

class Book {
public:
    struct data{
        char pk[20];
        char ISBN[20];
        char name[STRLEN];
        char author[STRLEN];
        char keyword[STRLEN];
        double price;
        int store;
        data(){
            strcpy(ISBN, "null");
            strcpy(name, "null");
            strcpy(author, "null");
            strcpy(keyword, "null");
            price = 0;
            store = 0;
        }
        bool operator==(const data& b){
            if(strcmp(pk, b.pk) != 0) return false;
            if(strcmp(ISBN, b.ISBN) != 0) return false;
            if(strcmp(name, b.name) != 0) return false;
            if(strcmp(author, b.author) != 0) return false;
            if(strcmp(keyword, b.keyword) != 0) return false;
            if(price != b.price) return false;
            return store == b.store;
        }
        void init(){
            strcpy(pk, "null");
            strcpy(ISBN, "null");
            strcpy(name, "null");
            strcpy(author, "null");
            strcpy(keyword, "null");
            price = 0;
            store = 0;
        }
        bool full_data(){
            if(strcmp(ISBN, "null") == 0) return false;
            if(strcmp(name, "null") == 0) return false;
            if(strcmp(author, "null") == 0) return false;
            return strcmp(keyword, "null") != 0;
        }
    };

private:
    const char *dbname = "Book.db";
    const char *db_r_ISBN = "book_reference_ISBN.db";
    const char *db_r_auth = "book_reference_author.db";
    const char *db_r_name = "book_reference_name.db";
    const char *db_r_keyword = "book_reference_keyword.db";
    BLL<char[20], data>* bkdb;

    Refer* r_ISBN;
    Refer* r_name;
    Refer* r_author;
    Refer* r_keyword;


    bool static cmp(const char a[STRLEN], const char b[STRLEN]) {
        return strcmp(a, b) < 0;
    }

    bool static equal(const char a[STRLEN], const char b[STRLEN]) {
        return strcmp(a, b) == 0;
    }


    void upref();
public:
    Book();
    bool addBook(data& ins);
    bool modify(char* ISBN, char* name, char* author, char* keyword, double price, char* oldISBN);
    bool exist(char* ISBN);
    data query(const char* ISBN);
    vector<data> get_all();
    vector<data> get_name(char *name);
    vector<data> get_author(char *author);
    vector<data> get_keyword(char *keyword);
    vector<data> get_ISBN(char *ISBN);
    bool sell_buy(const char* ISBN, int num);

    ~Book();

};

#endif //BOOKSTORE_BOOK_H
