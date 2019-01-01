//
// Created by pchen on 12/31/18.
//

#include "str.h"
#include <stdio.h>
#include <stdlib.h>

#define FREE(p)\
 do{ free((p)); (p) =  NULL; }while(0)

int main(int argc, char** argv){
    const char* str1 = "Hello";
    const char* str2 = "World";
    char* str3 = Str_cat(str1, 1, 3, str2, 2, 4);
    printf("%s\n", str3);
    FREE(str3);
}