//
// Created by pchen on 1/1/19.
//

#include "array.h"
#include <stdio.h>
#include <stdlib.h>

struct Complex{
    double r;
    double c;
};

#define SIZE 10

int main(int argc, char** argv){
    Array* array = Array_new(SIZE, sizeof(struct Complex));
    int i=0;
    for(i=0; i<SIZE; ++i){
        struct Complex * p = malloc(sizeof(*p));
        p->c=i;
        p->r=i+1;
        Array_put(array, i, p);
    }

    for(i=0; i<SIZE; ++i){
        struct Complex* p = Array_get(array, i);
        printf("Complex[%f, %f]\n", p->r, p->c);
    }

    Array_delete(&array);
    return 0;
}