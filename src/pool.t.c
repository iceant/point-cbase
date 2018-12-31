//
// Created by pchen on 12/31/18.
//

#include "pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stopwatch.h"

void* Demo_Malloc(unsigned long nBytes){
    void* p = malloc(nBytes);
    printf("\t...malloc(%d): %x\n", nBytes, p);
    return p;
}

void Demo_Free(void* ptr){
    printf("\t...delete(%x)\n", ptr);
    free(ptr);
}

typedef struct Complex{
    int r;
    int c;
}Complex;

int main(int argc, char** argv){
    printf("Complex size=%d\n", sizeof(Complex));


    Pool* pool = Pool_new(sizeof(Complex), 1000, Demo_Malloc, Demo_Free);
//    Pool* pool = Pool_NEW(sizeof(Complex), 1000);

    Complex* array[1000];

    Stopwatch* stopwatch = Stopwatch_new();
    Stopwatch_start(stopwatch);
    int i=0, j=0;
    for(i=0; i<500; i++){
        for(j=0; j<1000; j++){
            array[j] = (Complex*)Pool_alloc(pool);
        }
        for(j=0; j<1000; j++){
            Pool_free(pool, array[j]);
        }
    }
    Stopwatch_stop(stopwatch);
    printf("Time used in us: %.01f\n", Stopwatch_elapsedUS(stopwatch));
    printf("Time used in microseconds: %ld\n", Stopwatch_elapsedMS(stopwatch));
    Stopwatch_delete(&stopwatch);

    Pool_delete(&pool);
    assert(pool==NULL);
}