//
// Created by PCHEN on 2019/1/3.
//

#include "table.h"
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#define FREE(p)\
    free((p)); (p)=NULL;

struct Table {
    int size;

    int (*cmp)(const void *x, const void *y);

    unsigned (*hash)(const void *key);

    int length;
    unsigned timestamp;
    struct binding {
        struct binding *link;
        const void *key;
        void *value;
    } **buckets;
};

static int cmpatom(const void *x, const void *y) {
    return x != y;
}

static unsigned hashatom(const void *key) {
    return (unsigned long) key >> 2;
}

//////////////////////////////////////////////////////////////////////////////
//// Table Functions

Table *Table_new(int hint,
                 int cmp(const void *x, const void *y),
                 unsigned hash(const void *key)) {
    Table *table;
    int i;
    static int primes[] = {509, 509, 1021, 2053, 4093,
                           8191, 16381, 32771, 65521, INT_MAX};
    assert(hint >= 0);
    for (i = 1; primes[i] < hint; i++);
    table = malloc(sizeof(*table) +
                   primes[i - 1] * sizeof(table->buckets[0]));
    assert(table);
    table->size = primes[i - 1];
    table->cmp = cmp ? cmp : cmpatom;
    table->hash = hash ? hash : hashatom;
    table->buckets = (struct binding **) (table + 1);
    for (i = 0; i < table->size; i++)
        table->buckets[i] = NULL;
    table->length = 0;
    table->timestamp = 0;
    return table;
}


void Table_delete(Table **pTable) {
    assert(pTable && *pTable);
    if ((*pTable)->length > 0) {
        int i;
        struct binding *p, *q;
        for (i = 0; i < (*pTable)->size; i++)
            for (p = (*pTable)->buckets[i]; p; p = q) {
                q = p->link;
                FREE(p);
            }
    }
    FREE(*pTable);
}

int Table_length(Table *table) {
    assert(table);
    return table->length;
}

void *Table_get(Table *table, const void *key) {
    assert(table);
    assert(key);
    int i;
    struct binding *p;
    i = (*table->hash)(key) % table->size;
    for (p = table->buckets[i]; p; p = p->link)
        if ((*table->cmp)(key, p->key) == 0)
            break;
    return p ? p->value : NULL;
}

void *Table_put(Table *table, const void *key, void *value) {
    assert(table);
    assert(key);
    int i;
    struct binding *p;
    void *prev;
    i = (*table->hash)(key) % table->size;
    for (p = table->buckets[i]; p; p = p->link)
        if ((*table->cmp)(key, p->key) == 0)
            break;
    if (p == NULL) {
        p = malloc(sizeof(*p));
        assert(p);
        p->key = key;
        p->link = table->buckets[i];
        table->buckets[i] = p;
        table->length++;
        prev = NULL;
    } else
        prev = p->value;
    p->value = value;
    table->timestamp++;
    return prev;
}

void Table_each(Table *table, void (*apply)(const void *, void **, void *), void *cl) {
    assert(table);
    assert(apply);
    int i;
    unsigned stamp;
    struct binding *p;
    stamp = table->timestamp;
    for (i = 0; i < table->size; i++) {
        for (p = table->buckets[i]; p; p = p->link) {
            apply(p->key, &p->value, cl);
            assert(table->timestamp == stamp);
        }
    }
}

void *Table_remove(Table *table, const void *key) {
    assert(table);
    assert(key);
    int i;
    struct binding **pp;
    table->timestamp++;
    i = (*table->hash)(key) % table->size;
    for (pp = &table->buckets[i]; *pp; pp = &(*pp)->link)
        if ((*table->cmp)(key, (*pp)->key) == 0) {
            struct binding *p = *pp;
            void *value = p->value;
            *pp = p->link;
            FREE(p);
            table->length--;
            return value;
        }
    return NULL;
}

void **Table_toArray(Table *table, void *end) {
    assert(table);

    int i, j = 0;
    void **array;
    struct binding *p;
    array = malloc((2 * table->length + 1) * sizeof(*array));
    assert(array);
    for (i = 0; i < table->size; i++){
        for (p = table->buckets[i]; p; p = p->link) {
            array[j++] = (void *) p->key;
            array[j++] = p->value;
        }
    }
    array[j] = end;
    return array;
}


#undef FREE