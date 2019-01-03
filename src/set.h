// set.h                                                              -- C++ -- 
// Created by PCHEN on 2019/1/3.
// Copyright (c) 2019 POINTCX All rights reserved.
#ifndef POINT_CBASE_SET_H
#define POINT_CBASE_SET_H

typedef struct Set Set;

//////////////////////////////////////////////////////////////////////////////
//// CREATORS

Set *Set_new(int hint,
             int cmp(const void *x, const void *y),
             unsigned hash(const void *x));

void Set_delete(Set **pSet);

//////////////////////////////////////////////////////////////////////////////
//// MANIPULATORS

void Set_put(Set *set, const void *member);

void *Set_remove(Set *set, const void *member);

Set* Set_union(Set* s, Set* t);

Set* Set_inter(Set* s, Set* t);
    // cross part

Set* Set_minus(Set* s, Set* t);

Set* Set_diff(Set* s, Set* t);

void Set_each(Set *set, void apply(const void *member, void *cl), void *cl);

//////////////////////////////////////////////////////////////////////////////
//// ACCESSORS
int Set_length(Set *set);

int Set_member(Set *set, const void *member);
    // check whether member is part of set.

void** Set_toArray(Set* set, void* end);
    // void** array = Set_toArray(set, NULL);
    // ...
    // free(array); array = NULL; /* REMEMBER TO FREE 'array' */

#endif //POINT_CBASE_SET_H