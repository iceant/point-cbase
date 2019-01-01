//
// Created by pchen on 1/1/19.
//

#ifndef POINT_CBASE_ARRAY_H
#define POINT_CBASE_ARRAY_H

typedef struct Array Array;

//////////////////////////////////////////////////////////////////////////////
//// CREATORS
Array* Array_new(int length /*numOfElements*/, int sizeOfOneElement);
void Array_delete(Array**);

//////////////////////////////////////////////////////////////////////////////
//// MANIPULATORS
void* Array_put(Array* array, int idx, void* elem);

void Array_resize(Array* array, int length /*numOfElements*/);

Array* Array_copy(Array* array, int length /*numOfElements*/);

//////////////////////////////////////////////////////////////////////////////
//// ACCESSORS
int Array_length(Array*);
    // return the capacity of numOfElements in array

int Array_size(Array*);
    // return the elements size in array.

void* Array_get(Array* array, int idx);

#endif //POINT_CBASE_ARRAY_H
