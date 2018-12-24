//
// Created by tahara on 18-12-21.
//

#ifndef BOOKSTORE_REPORTER_H
#define BOOKSTORE_REPORTER_H

#include <cstring>
#include <cstdio>
#include <vector>

using std::vector;


template <typename T>
class Reporter {
private:
    char path[50];
    FILE *fp;
    int recorder_cnt;
    bool exist_file(const char* path);
    void oe();
    void cls();
public:
    Reporter(const char* path);
    void add_record(T ins);
    vector<T> get_record();
};

template<typename T>
Reporter<T>::Reporter(const char* path) {
    strcpy(this -> path, path);
    if(!exist_file(path)){
        recorder_cnt = 0;
        fp = fopen(path, "wb");
        fwrite(&recorder_cnt, sizeof(int), 1, fp);
        fclose(fp);
    }
}

template<typename T>
bool Reporter<T>::exist_file(const char *path) {
    FILE* ptr = fopen(path, "r");
    if(ptr){
        fclose(ptr);
        return true;
    }
    return false;
}


template<typename T>
void Reporter<T>::oe() {
    fp = fopen(path, "rb+");
    fseek(fp, 0, SEEK_SET);
    fread(&recorder_cnt, sizeof(int), 1, fp);
}

template<typename T>
void Reporter<T>::cls() {
    fseek(fp, 0, SEEK_SET);
    fwrite(&recorder_cnt, sizeof(int), 1, fp);
    fclose(fp);
}

template<typename T>
void Reporter<T>::add_record(T ins) {
    oe();
    recorder_cnt++;
    fseek(fp, 0, SEEK_END);
    fwrite(&ins, sizeof(T), 1, fp);
    cls();
}

template<typename T>
vector<T> Reporter<T>::get_record() {
    oe();
    vector<T> res;
    T tmp;
    int i;
    for(i = 0; i < recorder_cnt; i++){
        fseek(fp, sizeof(int) + sizeof(T) * i, SEEK_SET);
        fread(&tmp, sizeof(T), 1, fp);
        res.push_back(tmp);
    }
    cls();

    return res;
}

#endif //BOOKSTORE_REPORTER_H
