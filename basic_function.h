//
// Created by tahara on 18-12-23.
//

#ifndef BOOKSTORE_BASIC_FUNCTION_H
#define BOOKSTORE_BASIC_FUNCTION_H

#include <cstdio>

bool exist_file(const char*file){
    FILE* fptr = fopen(file, "r");
    if(fptr){
        fclose(fptr);
        return true;
    }
    return false;

}

#endif //BOOKSTORE_BASIC_FUNCTION_H
