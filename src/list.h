//
// Created by pchen on 1/1/19.
//

#ifndef POINT_CBASE_LIST_H
#define POINT_CBASE_LIST_H

typedef struct List {
    struct List * rest;
    void* first;
}List;

//////////////////////////////////////////////////////////////////////////////
//// CREATORS
List* List_new(void*, ...);
void List_delete(List** pList);

//////////////////////////////////////////////////////////////////////////////
//// MANIPULATORS
List* List_append(List* list, List* tail);

List* List_copy(List* list);

List* List_pop(List* list, void** x);

List* List_push(List* list, void* x);

List* List_reverse(List* list);

void List_each(List* list, void apply(void** x, void* cl), void* cl);

//////////////////////////////////////////////////////////////////////////////
//// ACCESSORS

int List_length(List* list);

void ** List_toArray(List* list, void * end);


#endif //POINT_CBASE_LIST_H
