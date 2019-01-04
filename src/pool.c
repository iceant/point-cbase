//
// Created by pchen on 12/31/18.
//

#include "pool.h"
#include "blocklist.h"

#include <stdlib.h>
#include <assert.h>

#define DEFAULT_CHUNK_SIZE 100

typedef struct Link{
    struct Link * d_next_p;
}Link;

struct Pool{
    struct BlockList * d_blockAllocator_p;
    unsigned int d_objectSize;
    unsigned int d_chunkSize;
    int d_instanceCount;
    struct Link* d_freeList_p;
};

void Pool_replenish(Pool* pool){
    unsigned long size = pool->d_chunkSize * pool->d_objectSize;
    unsigned char * start = (unsigned char*)BlockList_allocate(pool->d_blockAllocator_p, size);
    assert(start);
    unsigned char* last = &start[(pool->d_chunkSize-1)*pool->d_objectSize];
    unsigned  char* p = NULL;
    for(p = start; p<last; p+=pool->d_objectSize){
        ((struct Link*)p)->d_next_p = (struct Link*)(p+pool->d_objectSize);
    }
    ((struct Link*)last)->d_next_p = 0;
    pool->d_freeList_p = (struct Link*)start;
}

Pool* Pool_new(int objectSize, int chunkSize, void* (*mallocFn)(size_t), void (*freeFn)(void*)){
    Pool* pool = malloc(sizeof(*pool));
    assert(pool);
    pool->d_freeList_p = NULL;
    pool->d_objectSize = (objectSize>sizeof(struct Link))?objectSize:sizeof(struct Link);
    pool->d_chunkSize = chunkSize>0?chunkSize:DEFAULT_CHUNK_SIZE;
    pool->d_blockAllocator_p = BlockList_new(mallocFn, freeFn);
    pool->d_instanceCount = 0;
    return pool;
}

void Pool_delete(Pool** pPool){
    assert(pPool);
    assert(*pPool);

    if(0==(*pPool)->d_instanceCount){
        BlockList_delete(&(*pPool)->d_blockAllocator_p);
    }
    assert(0==(*pPool)->d_instanceCount);

    free(*pPool);
    *pPool = NULL;
}

void *Pool_alloc(Pool * pool) {
    assert(pool);

    if(!pool->d_freeList_p){
        Pool_replenish(pool);
    }

    struct Link* p = pool->d_freeList_p;
    pool->d_freeList_p = p->d_next_p;
    ++pool->d_instanceCount;
    return p;
}

void Pool_free(Pool* pool, void* obj){
    assert(pool);
    assert(obj);

    struct Link* p = (struct Link*)obj;
    p->d_next_p = pool->d_freeList_p;
    pool->d_freeList_p = p;
    --pool->d_instanceCount;
}

void Pool_dryUp(Pool * pool) {
    BlockList_release(pool->d_blockAllocator_p);
    pool->d_freeList_p = NULL;
}
