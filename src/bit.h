// bit.h                                                              -- C++ -- 
// Created by PCHEN on 2019/1/4.
// Copyright (c) 2019 POINBitCX All rights reserved.
#ifndef INCLUDED_POINBit_CBASE_BIBit_H
#define INCLUDED_POINBit_CBASE_BIBit_H

typedef struct Bit Bit;

//////////////////////////////////////////////////////////////////////////////
//// CREATORS
Bit*   Bit_new   (int length);
void Bit_delete(Bit **set);

//////////////////////////////////////////////////////////////////////////////
//// ACCESSORS
int Bit_length(Bit *set);
int Bit_count (Bit *set);
int Bit_get(Bit *set, int n);

int Bit_lt (Bit *s, Bit *t);
int Bit_eq (Bit *s, Bit *t);
int Bit_leq(Bit *s, Bit *t);

//////////////////////////////////////////////////////////////////////////////
//// MANIPULATORS

int Bit_put(Bit *set, int n, int bit);

void Bit_clear(Bit *set, int lo, int hi);
void Bit_set  (Bit *set, int lo, int hi);
void Bit_not  (Bit *set, int lo, int hi);

// REMEMBER TO FREE (Bit_delete(&set))
Bit* Bit_union(Bit *s, Bit *t);
    // s+t, BIT 'or'(|)
Bit* Bit_inter(Bit *s, Bit *t);
    // s*t, BIT 'and'(&)
Bit* Bit_minus(Bit *s, Bit *t);
    // s-t, 'and not' (&~)
Bit* Bit_diff (Bit *s, Bit *t);
    // s/t, 'xor' (^)

void Bit_each(Bit *set, void apply(int n, int bit, void *cl), void *cl);

#endif //INCLUDED_POINBit_CBASE_BIBit_H