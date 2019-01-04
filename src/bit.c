//
// Created by PCHEN on 2019/1/4.
//

#include "bit.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>//memcpy

//////////////////////////////////////////////////////////////////////////////
//// TYPE(s)
struct Bit{
    int length;
    unsigned char *bytes;
    unsigned long *words;
};

//////////////////////////////////////////////////////////////////////////////
//// DEFINE(s)
#define FREE(p)\
    free((p)); (p)=NULL;

#define BPW (8*sizeof (unsigned long))

#define nwords(len) ((((len) + BPW - 1)&(~(BPW-1)))/BPW)

#define nbytes(len) ((((len) + 8 - 1)&(~(8-1)))/8)

#define setop(sequal, snull, tnull, op) \
	if (s == t) { assert(s); return sequal; } \
	else if (s == NULL) { assert(t); return snull; } \
	else if (t == NULL) return tnull; \
	else { \
		int i; Bit* set; \
		assert(s->length == t->length); \
		set = Bit_new(s->length); \
		for (i = nwords(s->length); --i >= 0; ) \
			set->words[i] = s->words[i] op t->words[i]; \
		return set; }

//////////////////////////////////////////////////////////////////////////////
//// STATIC(s)

static unsigned char msbmask[] = {
        0xFF, 0xFE, 0xFC, 0xF8,
        0xF0, 0xE0, 0xC0, 0x80
};
static unsigned char lsbmask[] = {
        0x01, 0x03, 0x07, 0x0F,
        0x1F, 0x3F, 0x7F, 0xFF
};

static Bit* copy(Bit* t) {
    assert(t);
    Bit* set;
    set = Bit_new(t->length);
    if (t->length > 0)
        memcpy(set->bytes, t->bytes, nbytes(t->length));
    return set;
}

Bit* Bit_new(int length) {
    assert(length >= 0);
    Bit* set;
    set = malloc(sizeof(*set));
    assert(set);
    if (length > 0){
        set->words = calloc(nwords(length), sizeof (unsigned long));
        assert(set->words);
    }
    else
        set->words = NULL;
    set->bytes = (unsigned char *)set->words;
    set->length = length;
    return set;
}

void Bit_delete(Bit** pBit){
    assert(pBit && *pBit);
    FREE((*pBit)->words);
    FREE(*pBit);
}

int Bit_length(Bit *set) {
    assert(set);
    return set->length;
}

int Bit_count(Bit *set) {
    assert(set);
    int length = 0, n;
    static char count[] = { 0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4 };
    for (n = nbytes(set->length); --n >= 0; ) {
        unsigned char c = set->bytes[n];
        length += count[c&0xF] + count[c>>4];
    }
    return length;
}

int Bit_get(Bit *set, int n) {
    assert(set);
    assert(0 <= n && n < set->length);
    return ((set->bytes[n/8]>>(n%8))&1);
}

int Bit_lt(Bit *s, Bit *t) {
    assert(s && t);
    assert(s->length == t->length);
    int i, lt = 0;
    for (i = nwords(s->length); --i >= 0; )
        if ((s->words[i]&~t->words[i]) != 0)
            return 0;
        else if (s->words[i] != t->words[i])
            lt |= 1;
    return lt;
}

int Bit_eq(Bit *s, Bit *t) {
    assert(s && t);
    assert(s->length == t->length);
    int i;
    for (i = nwords(s->length); --i >= 0; )
        if (s->words[i] != t->words[i])
            return 0;
    return 1;
}

int Bit_leq(Bit *s, Bit *t) {
    assert(s && t);
    assert(s->length == t->length);
    int i;
    for (i = nwords(s->length); --i >= 0; )
        if ((s->words[i]&~t->words[i]) != 0)
            return 0;
    return 1;
}

int Bit_put(Bit *set, int n, int bit) {
    assert(set);
    assert(bit == 0 || bit == 1);
    assert(0 <= n && n < set->length);
    int prev;
    prev = ((set->bytes[n/8]>>(n%8))&1);
    if (bit == 1)
        set->bytes[n/8] |=   1<<(n%8);
    else
        set->bytes[n/8] &= ~(1<<(n%8));
    return prev;
}

void Bit_clear(Bit *set, int lo, int hi) {
    assert(set);
    assert(0 <= lo && hi < set->length);
    assert(lo <= hi);
    if (lo/8 < hi/8) {
        int i;
        set->bytes[lo/8] &= ~msbmask[lo%8];
        for (i = lo/8+1; i < hi/8; i++)
            set->bytes[i] = 0;
        set->bytes[hi/8] &= ~lsbmask[hi%8];
    } else
        set->bytes[lo/8] &= ~(msbmask[lo%8]&lsbmask[hi%8]);
}

void Bit_set(Bit *set, int lo, int hi) {
    assert(set);
    assert(0 <= lo && hi < set->length);
    assert(lo <= hi);
    if (lo/8 < hi/8) {
        set->bytes[lo/8] |= msbmask[lo%8];
        {
            int i;
            for (i = lo/8+1; i < hi/8; i++)
                set->bytes[i] = 0xFF;
        }
        set->bytes[hi/8] |= lsbmask[hi%8];
    } else
        set->bytes[lo/8] |= (msbmask[lo%8]&lsbmask[hi%8]);
}

void Bit_not(Bit *set, int lo, int hi) {
    assert(set);
    assert(0 <= lo && hi < set->length);
    assert(lo <= hi);
    if (lo/8 < hi/8) {
        int i;
        set->bytes[lo/8] ^= msbmask[lo%8];
        for (i = lo/8+1; i < hi/8; i++)
            set->bytes[i] ^= 0xFF;
        set->bytes[hi/8] ^= lsbmask[hi%8];
    } else
        set->bytes[lo/8] ^= (msbmask[lo%8]&lsbmask[hi%8]);
}

Bit *Bit_union(Bit *s, Bit *t) {
    setop(copy(t), copy(t), copy(s), |)
}

Bit *Bit_inter(Bit *s, Bit *t) {
    setop(copy(t), Bit_new(t->length), Bit_new(s->length), &)
}

Bit *Bit_minus(Bit *s, Bit *t) {
    setop(Bit_new(s->length), Bit_new(t->length), copy(s), & ~)
}

Bit *Bit_diff(Bit *s, Bit *t) {
    setop(Bit_new(s->length), copy(t), copy(s), ^)
}

void Bit_each(Bit *set, void (*apply)(int, int, void *), void *cl) {
    assert(set);
    assert(apply);
    int n;
    for (n = 0; n < set->length; n++)
        apply(n, ((set->bytes[n/8]>>(n%8))&1), cl);
}









