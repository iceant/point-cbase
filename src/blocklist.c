//
// Created by pchen on 12/31/18.
//
#include "blocklist.h"
#include <assert.h>
#include <stdlib.h>

typedef struct BlockLink{
    void * d_block_p;
    struct BlockLink* d_next_p;
}BlockLink;

struct BlockList{
    BlockLink* d_blockList_p; // linked list of allocated blocks.
    BlockList_malloc malloc;
    BlockList_free free;
};

#define BlockLink_DELETE(blockLink, blockList)\
    (blockList)->free((blockLink)->d_block_p);\
    (blockList)->free((blockLink));\
    (blockLink) = NULL;


BlockList* BlockList_new(BlockList_malloc mallocFn, BlockList_free freeFn){
    BlockList * p = mallocFn?mallocFn(sizeof(*p)):malloc(sizeof(*p));
    assert(p);
    p->d_blockList_p = NULL;
    p->malloc = mallocFn?mallocFn:malloc;
    p->free = freeFn?freeFn:free;
    return p;
}

void BlockList_delete(BlockList** pBlockList){
    assert(pBlockList);
    assert(*pBlockList);

    while((*pBlockList)->d_blockList_p){
        BlockLink* p = (*pBlockList)->d_blockList_p;
        (*pBlockList)->d_blockList_p = (*pBlockList)->d_blockList_p->d_next_p;
        BlockLink_DELETE(p, *pBlockList);
    }

    (*pBlockList)->free(*pBlockList);
    *pBlockList=NULL;
}

void BlockList_release(BlockList* blockList){
    assert(blockList);
    while(blockList->d_blockList_p){
        BlockLink* p = blockList->d_blockList_p;
        blockList->d_blockList_p = blockList->d_blockList_p->d_next_p;
        BlockLink_DELETE(p, blockList);
    }
}

void* BlockList_allocate(BlockList* blockList, unsigned long nBytes){
    void* p = blockList->malloc(nBytes);
    assert(p);
    BlockLink * link = blockList->malloc(sizeof(*link));
    assert(link);
    link->d_block_p = p;
    link->d_next_p = blockList->d_blockList_p;
    blockList->d_blockList_p = link;
    return p;
}