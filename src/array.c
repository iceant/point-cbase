//
// Created by pchen on 1/1/19.
//

#include "array.h"
#include <stdlib.h>
#include <string.h> // memcpy
#include <assert.h>

struct Array{
    int length;
    int size;
    char* array;
};

void ArrayRep_init(Array* array, int length, int size, void *ary){
    assert(array);
    assert(ary && length>0 || length==0 && ary==NULL);
    assert(size > 0);
    array->length = length;
    array->size   = size;
    if (length > 0)
        array->array = ary;
    else
        array->array = NULL;
}


//////////////////////////////////////////////////////////////////////////////
////
Array *Array_new(int length, int size) {
    Array* array = malloc(sizeof(*array));
    assert(array);
    if (length > 0){
        void * ary = calloc(length, size);
        assert(ary);
        ArrayRep_init(array, length, size, ary);
    }else
        ArrayRep_init(array, length, size, NULL);
    return array;
}

void Array_delete(Array ** pArray) {
    assert(pArray);
    assert(*pArray);
    free((*pArray)->array);
    (*pArray)->array = NULL;
    free(*pArray);
    *pArray = NULL;
}

void *Array_put(Array *array, int idx, void *elem) {
    assert(array);
    assert(idx >= 0 && idx < array->length);
    assert(elem);
    memcpy(array->array + idx * array->size, elem, array->size);
    return elem;
}

void Array_resize(Array *array, int length) {
    assert(array);
    assert(length >= 0);
    if (length == 0){
        free(array->array); array->array = NULL;
    }
    else if (array->length == 0){
        array->array = malloc(length*array->size);
        assert(array->array);
    }
    else{
        array->array = realloc(array->array, length * array->size);
        assert(array->array);
    }
    array->length = length;
}

Array *Array_copy(Array *array, int length) {
    Array* copy;
    assert(array);
    assert(length >= 0);
    copy = Array_new(length, array->size);
    if (copy->length >= array->length && array->length > 0)
        memcpy(copy->array, array->array, array->length*array->size);
    else if (array->length > copy->length && copy->length > 0)
        memcpy(copy->array, array->array, copy->length*array->size);
    return copy;
}

int Array_length(Array * array) {
    assert(array);
    return array->length;
}

int Array_size(Array * array) {
    assert(array);
    return array->size;
}

void *Array_get(Array *array, int i) {
    assert(array);
    assert(i >= 0 && i < array->length);
    return array->array + i*array->size;
}







