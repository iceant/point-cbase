//
// Created by PCHEN on 2019/1/4.
//

#include "stack.h"
#include <stdlib.h>
#include <assert.h>

struct Stack {
    int count;
    struct elem {
        void *x;
        struct elem *link;
    } *head;
};

#define FREE(p)\
    free((p)); (p) = NULL;

Stack *Stack_new(void) {
    Stack* stk;
    stk = malloc(sizeof(*stk));
    assert(stk);
    stk->count = 0;
    stk->head = NULL;
    return stk;
}

void Stack_delete(Stack **stk) {
    struct elem *t, *u;
    assert(stk && *stk);
    for (t = (*stk)->head; t; t = u) {
        u = t->link;
        FREE(t);
    }
    FREE(*stk);
}

void Stack_push(Stack *stk, void *x) {
    assert(stk);
    struct elem *t;
    t = malloc(sizeof(*t));
    assert(t);
    t->x = x;
    t->link = stk->head;
    stk->head = t;
    stk->count++;
}

void *Stack_pop(Stack *stk) {
    void *x;
    struct elem *t;
    assert(stk);
    assert(stk->count > 0);
    t = stk->head;
    stk->head = t->link;
    stk->count--;
    x = t->x;
    FREE(t);
    return x;
}

void *Stack_top(Stack *stk) {
    assert(stk);
    assert(stk->count>0);
    
    struct elem * t;
    t = stk->head;
    return t->x;
}

int Stack_isEmpty(Stack *stk) {
    assert(stk);
    return stk->count == 0;
}






