// table.h                                                            -- C++ -- 
// Created by PCHEN on 2019/1/3.
// Copyright (c) 2019 POINTCX All rights reserved.
#ifndef POINT_CBASE_TABLE_H
#define POINT_CBASE_TABLE_H

typedef struct Table Table;

//////////////////////////////////////////////////////////////////////////////
//// CREATORS
Table *Table_new(int hint,
                 int cmp(const void *x, const void *y),
                 unsigned hash(const void *key));

void Table_delete(Table **pTable);

//////////////////////////////////////////////////////////////////////////////
//// MANIPULATORS
void *Table_put(Table *table, const void *key, void *value);

void *Table_remove(Table *table, const void *key);

//////////////////////////////////////////////////////////////////////////////
//// ACCESSORS
int Table_length(Table *table);

void *Table_get(Table *table, const void *key);


void Table_each(Table *table,
                void apply(const void *key, void **value, void *cl),
                void *cl);

void **Table_toArray(Table *table, void *end);

#endif //POINT_CBASE_TABLE_H