// atom.h                                                             -- C++ -- 
// Created by PCHEN on 2019/1/3.
// Copyright (c) 2019 POINTCX All rights reserved.
#ifndef INCLUDED_POINT_CBASE_ATOM_H
#define INCLUDED_POINT_CBASE_ATOM_H

int Atom_length(const char *str);

const char *Atom_new(const char *str, int len);

const char *Atom_string(const char *str);

const char *Atom_int(long n);

const char *Atom_int64(long long n);


#endif //INCLUDED_POINT_CBASE_ATOM_H