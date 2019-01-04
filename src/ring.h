// ring.h                                                             -- C++ -- 
// Created by PCHEN on 2019/1/4.
// Copyright (c) 2019 POINTCX All rights reserved.
#ifndef POINT_CBASE_RING_H
#define POINT_CBASE_RING_H

typedef struct Ring Ring;

//////////////////////////////////////////////////////////////////////////////
//// CREATORS
Ring* Ring_new (void);
    // Create an empty Ring
    
Ring* Ring_ring(void *x, ...);

void Ring_delete(Ring **ring);

//////////////////////////////////////////////////////////////////////////////
//// MANIPULATORS

void *Ring_put(Ring* ring, int i, void *x);
void *Ring_add(Ring* ring, int pos, void *x);
void *Ring_addlo(Ring* ring, void *x);
void *Ring_addhi(Ring* ring, void *x);
void *Ring_remove(Ring* ring, int i);
void *Ring_rmlo(Ring* ring);
void *Ring_rmhi(Ring* ring);
void Ring_rotate(Ring* ring, int n);


//////////////////////////////////////////////////////////////////////////////
//// ACCESSORS

int  Ring_length(Ring*  ring);
void *Ring_get(Ring* ring, int i);


#endif //POINT_CBASE_RING_H