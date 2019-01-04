//
// Created by PCHEN on 2019/1/4.
//

#include "bit.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void apply(int idx, int bit, void* cl){
    printf("bit(%02d)=%d\n", idx, bit);
}

int main(int argc, char** argv){
    Bit* bit = Bit_new(32);
    
    assert(Bit_length(bit)==32);
    
    Bit_put(bit, 2, 1);
    Bit_put(bit, 30, 1);
    assert(Bit_count(bit)==2);
    
    assert(Bit_get(bit, 2)==1);
    assert(Bit_get(bit, 30)==1);
    assert(Bit_get(bit, 0)==0);
    assert(Bit_get(bit, 31)==0);
    
    Bit_clear(bit, 0, 31);
    assert(Bit_get(bit, 2)==0);
    assert(Bit_get(bit, 30)==0);
    
    Bit_set(bit, 0, 31);
    assert(Bit_get(bit, 2)==1);
    assert(Bit_get(bit, 30)==1);
    
    Bit_not(bit, 0, 31);
    assert(Bit_get(bit, 2)==0);
    assert(Bit_get(bit, 30)==0);
    
    Bit* bit2 = Bit_new(32);
    Bit_set(bit2, 0, 31);
    assert(Bit_lt(bit, bit2));
    assert(!Bit_lt(bit2, bit));
    
    Bit_set(bit, 0, 31);
    assert(Bit_eq(bit, bit2));
    assert(Bit_leq(bit, bit2));
    
    Bit_each(bit, apply, NULL);
    
    Bit_clear(bit, 0, 31);
    Bit_clear(bit2, 0, 31);
    
    Bit_set(bit, 5, 10);
    Bit_set(bit2, 7, 20);
    Bit * bit3 = Bit_union(bit, bit2);
    printf("bit union bit2...\n");
    Bit_each(bit3, apply, NULL);
    Bit_delete(&bit3);
    
    bit3 = Bit_minus(bit, bit2);
    printf("bit minus bit2...\n");
    Bit_each(bit3, apply, NULL);
    Bit_delete(&bit3);
    
    bit3 = Bit_inter(bit, bit2);
    printf("bit inter bit2...\n");
    Bit_each(bit3, apply, NULL);
    Bit_delete(&bit3);
    
    bit3 = Bit_diff(bit, bit2);
    printf("bit diff bit2...\n");
    Bit_each(bit3, apply, NULL);
    Bit_delete(&bit3);
    
    
    Bit_delete(&bit2);
    Bit_delete(&bit);
    assert(bit==NULL);
}