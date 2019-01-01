//
// Created by pchen on 12/31/18.
//
#include "fmt.h"
#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

int main(int argc, char** argv){
    int i=0;
    char* array[SIZE];
    for(i=0; i<SIZE; i++){
        array[i] = Fmt_string("Fmt_string_COUNT - %d", i);
    }
    for(i=0; i<SIZE; i++){
        printf("%s\n", array[i]);
        free(array[i]);
        array[i]=NULL;
    }
}
