//
// Created by pchen on 12/31/18.
//

#ifndef POINT_CBASE_BLOCKLIST_H
#define POINT_CBASE_BLOCKLIST_H

#ifndef INCLUDED_STDDEF_H
#define INCLUDED_STDDEF_H
#include <stddef.h>
#endif //INCLUDED_STDDEF_H

typedef struct BlockList BlockList;

typedef void* (*BlockList_malloc)(size_t);
typedef void (*BlockList_free)(void*);

BlockList* BlockList_new(BlockList_malloc, BlockList_free);
    // Create an empty list of allocated blocks of memory

void BlockList_delete(BlockList**);
    // Destroy the object and all associated blocks of memory.

void* BlockList_allocate(BlockList*, size_t nBytes);
    // Allocate block of memory of specified number of bytes

void BlockList_release(BlockList*);
    // Free all blocks of memory allocated through this object.

#define BlockList_NEW()\
    BlockList_new(NULL, NULL)

#endif //POINT_CBASE_BLOCKLIST_H
