//
// Created by pchen on 1/1/19.
//

#include "list.h"
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

List *List_new(void * x, ...) {
    va_list ap;
    List* list, **p = &list;
    va_start(ap, x);
    for ( ; x; x = va_arg(ap, void *)) {
        *p = malloc(sizeof(*(*p)));
        assert(*p);
        (*p)->first = x;
        p = &(*p)->rest;
    }
    *p = NULL;
    va_end(ap);
    return list;
}

void List_delete(List **pList) {
    List* next;
    assert(pList);
    for ( ; *pList; *pList = next) {
        next = (*pList)->rest;
        if(*pList){
            free(*pList);
            *pList = NULL;
        }
    }
}

List *List_append(List *list, List *tail) {
    List* *p = &list;
    while (*p)
        p = &(*p)->rest;
    *p = tail;
    return list;
}

List *List_copy(List *list) {
    List* head, **p = &head;
    for ( ; list; list = list->rest) {
        *p = malloc(sizeof(*(*p)));
        assert(*p);
        (*p)->first = list->first;
        p = &(*p)->rest;
    }
    *p = NULL;
    return head;
}

List *List_pop(List *list, void **x) {
    if (list) {
        List* head = list->rest;
        if (x)
            *x = list->first;
        free(list);
        return head;
    } else
        return list;
}

List *List_push(List *list, void *x) {
    List* p;
    p = malloc(sizeof(*p));
    assert(p);
    p->first = x;
    p->rest  = list;
    return p;
}

List *List_reverse(List *list) {
    List* head = NULL, *next;
    for ( ; list; list = next) {
        next = list->rest;
        list->rest = head;
        head = list;
    }
    return head;
}

void List_each(List *list, void (*apply)(void **, void *), void *cl) {
    assert(apply);
    for ( ; list; list = list->rest)
        apply(&list->first, cl);
}

int List_length(List *list) {
    int n;
    for (n = 0; list; list = list->rest)
        n++;
    return n;
}

void **List_toArray(List *list, void *end) {
    int i, n = List_length(list);
    void **array = malloc((n + 1)*sizeof (*array));
    assert(array);
    for (i = 0; i < n; i++) {
        array[i] = list->first;
        list = list->rest;
    }
    array[i] = end;
    return array;
}



