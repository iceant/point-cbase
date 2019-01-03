//
// Created by PCHEN on 2019/1/3.
//

#include "set.h"
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#define FREE(p)\
    free((p)); (p)=NULL;

struct Set {
    int      length;
    unsigned timestamp;
    
    int (*cmp)(const void *x, const void *y);
    
    unsigned (*hash)(const void *x);
    
    int size;
    struct member {
        struct member *link;
        const void    *member;
    }   **buckets;
};

static int cmpatom(const void *x, const void *y) {
    return x != y;
}

static unsigned hashatom(const void *x) {
    return (unsigned long) x >> 2;
}

static Set *Set_copy(Set *t, int hint) {
    assert(t);
    
    Set *set;
    
    set = Set_new(hint, t->cmp, t->hash);
    {
        int           i;
        struct member *q;
        for (i = 0; i < t->size; i++){
            for (q = t->buckets[i]; q; q = q->link) {
                struct member *p;
                const void    *member = q->member;
                int           idx     = (*set->hash)(member) % set->size;
                p = malloc(sizeof(*p));
                assert(p);
                p->member = member;
                p->link   = set->buckets[idx];
                set->buckets[idx] = p;
                set->length++;
            }
        }
    }
    return set;
}

static int Arith_max(int x, int y) {
    return x > y ? x : y;
}

static int Arith_min(int x, int y) {
    return x > y ? y : x;
}

//////////////////////////////////////////////////////////////////////////////
////
Set *Set_new(int hint, int (*cmp)(const void *, const void *),
             unsigned int (*hash)(const void *)) {
    assert(hint >= 0);
    
    Set        *set;
    int        i;
    static int primes[] = {509, 509, 1021, 2053, 4093,
                           8191, 16381, 32771, 65521, INT_MAX};
    for (i = 1; primes[i] < hint; i++);
    set    = malloc(sizeof(*set) + primes[i - 1] * sizeof(set->buckets[0]));
    assert(set);
    
    set->length    = 0;
    set->timestamp = 0;
    set->size    = primes[i - 1];
    set->cmp     = cmp ? cmp : cmpatom;
    set->hash    = hash ? hash : hashatom;
    set->buckets = (struct member **) (set + 1);
    for (i = 0; i < set->size; i++) {
        set->buckets[i] = NULL;
    }
    return set;
}

void Set_delete(Set **set) {
    assert(set && *set);
    if ((*set)->length > 0) {
        int           i;
        struct member *p, *q;
        for (i = 0; i < (*set)->size; i++) {
            for (p = (*set)->buckets[i]; p; p = q) {
                q = p->link;
                FREE(p);
            }
        }
    }
    FREE(*set);
}

void Set_put(Set *set, const void *member) {
    assert(set);
    assert(member);
    
    int           i;
    struct member *p;
    
    i      = (*set->hash)(member) % set->size;
    for (p = set->buckets[i]; p; p = p->link){
        if ((*set->cmp)(member, p->member) == 0){
            break;
        }
    }
    if (p == NULL) {
        p = malloc(sizeof(*p));
        assert(p);
        p->member = member;
        p->link   = set->buckets[i];
        set->buckets[i] = p;
        set->length++;
    } else
        p->member = member;
    set->timestamp++;
}

void *Set_remove(Set *set, const void *member) {
    assert(set);
    assert(member);
    
    int           i;
    struct member **pp;
    
    set->timestamp++;
    i       = (*set->hash)(member) % set->size;
    for (pp = &set->buckets[i]; *pp; pp = &(*pp)->link){
        if ((*set->cmp)(member, (*pp)->member) == 0) {
            struct member *p = *pp;
            *pp = p->link;
            member = p->member; /* d_value_p */
            FREE(p);
            set->length--;
            return (void *) member;
        }
    }
    return NULL;
}

Set *Set_union(Set *s, Set *t) {
    if (s == NULL) {
        assert(t);
        return Set_copy(t, t->size);
    } else if (t == NULL)
        return Set_copy(s, s->size);
    else {
        Set *set = Set_copy(s, Arith_max(s->size, t->size));
        assert(s->cmp == t->cmp && s->hash == t->hash);
        {
            int           i;
            struct member *q;
            for (i = 0; i < t->size; i++) {
                for (q = t->buckets[i]; q; q = q->link) {
                    Set_put(set, q->member);
                }
            }
        }
        return set;
    }
}

Set *Set_inter(Set *s, Set *t) {
    if (s == NULL) {
        assert(t);
        return Set_new(t->size, t->cmp, t->hash);
    } else if (t == NULL)
        return Set_new(s->size, s->cmp, s->hash);
    else if (s->length < t->length)
        return Set_inter(t, s);
    else {
        Set *set = Set_new(Arith_min(s->size, t->size), s->cmp, s->hash);
        assert(s->cmp == t->cmp && s->hash == t->hash);
        {
            int           i;
            struct member *q;
            for (i = 0; i < t->size; i++){
                for (q = t->buckets[i]; q; q = q->link){
                    if (Set_member(s, q->member)) {
                        struct member *p;
                        const void    *member = q->member;
                        int idx  = (*set->hash)(member) % set->size;
                        p = malloc(sizeof(*p));
                        assert(p);
                        p->member = member;
                        p->link   = set->buckets[idx];
                        set->buckets[idx] = p;
                        set->length++;
                    }
                }
            }
        }
        return set;
    }
}

Set *Set_minus(Set *t, Set *s) {
    if (t == NULL) {
        assert(s);
        return Set_new(s->size, s->cmp, s->hash);
    } else if (s == NULL)
        return Set_copy(t, t->size);
    else {
        Set *set = Set_new(Arith_min(s->size, t->size), s->cmp, s->hash);
        assert(s->cmp == t->cmp && s->hash == t->hash);
        {
            int           i;
            struct member *q;
            for (i = 0; i < t->size; i++) {
                for (q = t->buckets[i]; q; q = q->link) {
                    if (!Set_member(s, q->member)) {
                        struct member *p;
                        const void    *member = q->member;
                        int idx = (*set->hash)(member) % set->size;
                        p = malloc(sizeof(*p));
                        assert(p);
                        p->member = member;
                        p->link   = set->buckets[idx];
                        set->buckets[idx] = p;
                        set->length++;
                    }
                }
            }
        }
        return set;
    }
}

Set *Set_diff(Set *s, Set *t) {
    if (s == NULL) {
        assert(t);
        return Set_copy(t, t->size);
    } else if (t == NULL) {
        return Set_copy(s, s->size);
    } else {
        Set *set = Set_new(Arith_min(s->size, t->size), s->cmp, s->hash);
        assert(s->cmp == t->cmp && s->hash == t->hash);
        {
            int           i;
            struct member *q;
            for (i = 0; i < t->size; i++) {
                for (q = t->buckets[i]; q; q = q->link) {
                    if (!Set_member(s, q->member)) {
                        struct member *p;
                        const void    *member = q->member;
                        int idx = (*set->hash)(member) % set->size;
                        p = malloc(sizeof(*p));
                        assert(p);
                        p->member = member;
                        p->link   = set->buckets[idx];
                        set->buckets[idx] = p;
                        set->length++;
                    }
                }
            }
        }
        { Set *u = t;t = s;s = u; }
        {
            int           i;
            struct member *q;
            for (i = 0; i < t->size; i++) {
                for (q = t->buckets[i]; q; q = q->link) {
                    if (!Set_member(s, q->member)) {
                        struct member *p;
                        const void    *member = q->member;
                        int idx = (*set->hash)(member) % set->size;
                        p = malloc(sizeof(*p));
                        assert(p);
                        p->member = member;
                        p->link   = set->buckets[idx];
                        set->buckets[idx] = p;
                        set->length++;
                    }
                }
            }
        }
        return set;
    }
}

void Set_each(Set *set, void (*apply)(const void *, void *), void *cl) {
    assert(set);
    assert(apply);
    
    int           i;
    unsigned      stamp;
    struct member *p;
    stamp  = set->timestamp;
    for (i = 0; i < set->size; i++) {
        for (p = set->buckets[i]; p; p = p->link) {
            apply(p->member, cl);
            assert(set->timestamp == stamp);
        }
    }
}

int Set_length(Set *set) {
    assert(set);
    return set->length;
}

int Set_member(Set *set, const void *member) {
    assert(set);
    assert(member);
    
    int           i;
    struct member *p;
    i      = (*set->hash)(member) % set->size;
    for (p = set->buckets[i]; p; p = p->link) {
        if ((*set->cmp)(member, p->member) == 0) {
            break;
        }
    }
    return p != NULL;
}

void **Set_toArray(Set *set, void *end) {
    assert(set);
    
    int           i, j = 0;
    void          **array;
    struct member *p;
    
    array = malloc((set->length + 1) * sizeof(*array));
    assert(array);
    for (i   = 0; i < set->size; i++) {
        for (p = set->buckets[i]; p; p = p->link) {
            array[j++] = (void *) p->member;
        }
    }
    array[j] = end;
    return array;
}


