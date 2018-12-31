//
// Created by pchen on 12/30/18.
//

#include "scopeguard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset

#include "hexdump.c"

#define SIZE 32

void ScopeGuard_FreeMemory(void* mem_p){
    printf("free memory at %x\n", mem_p);
    free(mem_p);
    hexdump(mem_p, SIZE);
}

int main(int argc, char** argv){
    ScopeGuard * scope_p = ScopeGuard_new();

    void* mem_p = malloc(SIZE);
    printf("allocated memory at %x\n", mem_p);
    ScopeGuard_onExit(scope_p, ScopeGuard_FreeMemory, mem_p);
    memset(mem_p, 1, SIZE);
    hexdump(mem_p, SIZE);

    void * mem_p2 = malloc(SIZE*2);
    printf("allocated memory at %x\n", mem_p2);
    ScopeGuard_onExit(scope_p, ScopeGuard_FreeMemory, mem_p2);

//    ScopeGuard_dismiss(scope_p);

    if(ScopeGuard_isDismissed(scope_p)){
        free(mem_p);
        free(mem_p2);
    }

    ScopeGuard_exit(&scope_p);


}