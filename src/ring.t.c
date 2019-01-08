//
// Created by PCHEN on 2019/1/4.
//

#include "ring.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define Ring_addIntValue(ring, val)\
    do{ int * p = malloc(sizeof(*p)); assert(p); *p = val; Ring_addhi(ring, p); }while(0)

#define Ring_deleteVal(ring, pos)\
    do{ int* p = (int*)Ring_remove(ring, pos); free(p); p = NULL; } while(0)
    
int main(int argc, char** argv){
    Ring * ring = Ring_new();
    assert(Ring_length(ring)==0);
    
    Ring_addIntValue(ring, 0);
    Ring_addIntValue(ring, 1);
    Ring_addIntValue(ring, 2);
    Ring_addIntValue(ring, 3);
    Ring_addIntValue(ring, 4);
    
    printf("ring length: %d\n", Ring_length(ring));
    
    int i=0;
    
    for(i=0; i<Ring_length(ring); i++) {
        int *p = Ring_get(ring, i);
        printf("\tring(%d)=%d\n", i, *p);
    }
    
    printf("rotate -3\n");
    Ring_rotate(ring, -3);
    int *p = Ring_get(ring, 0);
    printf("\tring(0)=%d\n", *p);
    
    printf("rotate 2\n");
    Ring_rotate(ring, 2);
    p = Ring_get(ring, 0);
    printf("\tring(0)=%d\n", *p);
    
    int ringLength = Ring_length(ring);
    for(i=0; i<ringLength; i++){
        Ring_deleteVal(ring, 0);
    }
    
    Ring_delete(&ring);
    
    return 0;
}