// getword.h                                                          -- C++ -- 
// Created by PCHEN on 2019/1/3.
// Copyright (c) 2019 POINTCX All rights reserved.
#ifndef INCLUDED_POINT_CBASE_GETWORD_H
#define INCLUDED_POINT_CBASE_GETWORD_H

#ifndef INCLUDED_STDIO_H
#define INCLUDED_STDIO_H
#include <stdio.h>
#endif //INCLUDED_STDIO_H

extern int getword(FILE *fp, char *buf, int size,
                   int first(int c), int rest(int c));


#endif //INCLUDED_POINT_CBASE_GETWORD_H