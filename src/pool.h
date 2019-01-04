//
// Created by pchen on 12/31/18.
//

#ifndef POINT_CBASE_POOL_H
#define POINT_CBASE_POOL_H

#ifndef INCLUDED_STDDEF_H
#define INCLUDED_STDDEF_H
#include <stddef.h>
#endif //INCLUDED_STDDEF_H

typedef struct Pool Pool;

Pool* Pool_new(int objectSize, int chunkSize, void* (*mallocFn)(size_t), void (*freeFn)(void*));
    // Create an allocator for a pool of specified size

void Pool_delete(Pool**);
    // Destroy pool AND ALL ASSOCIATED BLOCKS OF MEMORY

void* Pool_alloc(Pool*);
    // Allocate block of memory of specified number of bytes

void Pool_free(Pool*, void*);

void Pool_dryUp(Pool*);

#define Pool_NEW(objectSize, chunkSize)\
    Pool_new(objectSize, chunkSize, NULL, NULL)

#endif //POINT_CBASE_POOL_H
