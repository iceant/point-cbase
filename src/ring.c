//
// Created by PCHEN on 2019/1/4.
//

#include "ring.h"
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

struct Ring {
    struct node {
        struct node *llink, *rlink;
        void        *value;
    }   *head;
    int length;
};


#define FREE(p)\
    free((p));(p)=NULL;

Ring *Ring_new(void) {
    Ring *ring;
    ring = malloc(sizeof(*ring));
    assert(ring);
    ring->head   = NULL;
    ring->length = 0;
    return ring;
}

Ring *Ring_ring(void *x, ...) {
    va_list ap;
    Ring *ring  = Ring_new();
    va_start(ap, x);
    for (; x; x = va_arg(ap, void *))
        Ring_addhi(ring, x);
    va_end(ap);
    return ring;
}

void Ring_delete(Ring **ring) {
    assert(ring && *ring);
    struct node *p, *q;
    if ((p = (*ring)->head) != NULL) {
        int n = (*ring)->length;
        for ( ; n-- > 0; p = q) {
            q = p->rlink;
            FREE(p);
        }
    }
    FREE(*ring);
}

int Ring_length(Ring *ring) {
    assert(ring);
    return ring->length;
}

void *Ring_get(Ring *ring, int i) {
    assert(ring);
    assert(i >= 0 && i < ring->length);
    struct node *q;
    {
        int n;
        q = ring->head;
        if (i <= ring->length/2) {
            for (n = i; n-- > 0;){
                q = q->rlink;
            }
        }
        else{
            for (n = ring->length - i; n-- > 0; ){
                q = q->llink;
            }
        }
    }
    return q->value;
}

void *Ring_put(Ring *ring, int i, void *x) {
    assert(ring);
    assert(i >= 0 && i < ring->length);
    struct node *q;
    void *prev;
    {
        int n;
        q = ring->head;
        if (i <= ring->length/2)
            for (n = i; n-- > 0; )
                q = q->rlink;
        else
            for (n = ring->length - i; n-- > 0; )
                q = q->llink;
    }
    prev = q->value;
    q->value = x;
    return prev;
}

void *Ring_add(Ring *ring, int pos, void *x) {
    assert(ring);
    assert(pos >= -ring->length && pos<=ring->length+1);
    if (pos == 1 || pos == -ring->length)
        return Ring_addlo(ring, x);
    else if (pos == 0 || pos == ring->length + 1)
        return Ring_addhi(ring, x);
    else {
        struct node *p, *q;
        int i = pos < 0 ? pos + ring->length : pos - 1;
        {
            int n;
            q = ring->head;
            if (i <= ring->length/2)
                for (n = i; n-- > 0; )
                    q = q->rlink;
            else
                for (n = ring->length - i; n-- > 0; )
                    q = q->llink;
        }
        p = malloc(sizeof(*p));
        assert(p);
        {
            p->llink = q->llink;
            q->llink->rlink = p;
            p->rlink = q;
            q->llink = p;
        }
        ring->length++;
        return p->value = x;
    }
}

void *Ring_addlo(Ring *ring, void *x) {
    assert(ring);
    Ring_addhi(ring, x);
    ring->head = ring->head->llink;
    return x;
}

void *Ring_addhi(Ring *ring, void *x) {
    struct node *p, *q;
    assert(ring);
    p = malloc(sizeof(*p));
    assert(p);
    if ((q = ring->head) != NULL)
    {
        p->llink = q->llink;
        q->llink->rlink = p;
        p->rlink = q;
        q->llink = p;
    }
    else
        ring->head = p->llink = p->rlink = p;
    ring->length++;
    return p->value = x;
}

void *Ring_remove(Ring *ring, int i) {
    assert(ring);
    assert(ring->length > 0);
    assert(i >= 0 && i < ring->length);
    void *x;
    struct node *q;
    {
        int n;
        q = ring->head;
        if (i <= ring->length/2)
            for (n = i; n-- > 0; )
                q = q->rlink;
        else
            for (n = ring->length - i; n-- > 0; )
                q = q->llink;
    }
    if (i == 0)
        ring->head = ring->head->rlink;
    x = q->value;
    q->llink->rlink = q->rlink;
    q->rlink->llink = q->llink;
    FREE(q);
    if (--ring->length == 0)
        ring->head = NULL;
    return x;
}

void *Ring_rmlo(Ring *ring) {
    assert(ring);
    assert(ring->length > 0);
    ring->head = ring->head->rlink;
    return Ring_rmhi(ring);
}

void *Ring_rmhi(Ring *ring) {
    assert(ring);
    assert(ring->length > 0);
    void *x;
    struct node *q;
    q = ring->head->llink;
    x = q->value;
    q->llink->rlink = q->rlink;
    q->rlink->llink = q->llink;
    FREE(q);
    if (--ring->length == 0)
        ring->head = NULL;
    return x;
}

void Ring_rotate(Ring *ring, int n) {
    assert(ring);
    assert(n >= -ring->length && n <= ring->length);
    struct node *q;
    int i;
    if (n >= 0)
        i = n%ring->length;
    else
        i = n + ring->length;
    {
        int n;
        q = ring->head;
        if (i <= ring->length/2)
            for (n = i; n-- > 0; )
                q = q->rlink;
        else
            for (n = ring->length - i; n-- > 0; )
                q = q->llink;
    }
    ring->head = q;
}




