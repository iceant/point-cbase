//
// Created by pchen on 12/31/18.
//

#include "blocklist.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

void* Demo_Malloc(size_t nBytes){
    void* p = malloc(nBytes);
    printf("\t...malloc(%d): %x\n", nBytes, p);
    return p;
}

void Demo_Free(void* ptr){
    printf("\t...delete(%x)\n", ptr);
    free(ptr);
}

int main(int argc, char** argv){
    printf("BlockList b();\n");
    BlockList* b = BlockList_new(Demo_Malloc, Demo_Free);
//    BlockList* b = BlockList_NEW();

    printf("b.allocate(100);\n");
    BlockList_allocate(b, 100);

    printf("b.allocate(200);\n");
    BlockList_allocate(b, 200);

    printf("b.release();\n");
    BlockList_release(b);

    printf("b.allocate(100);\n");
    BlockList_allocate(b, 100);

    BlockList_delete(&b);
    assert(b==NULL);
}