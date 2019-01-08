// fnutil.h                                                           -- C++ -- 
// Created by PCHEN on 2019/1/7.
// Copyright (c) 2019 POINTCX All rights reserved.
#ifndef POINT_CBASE_FNUTIL_H
#define POINT_CBASE_FNUTIL_H

#define Fn_apply(type, fn, ...){ int i;                                     \
    void * stopper_for_apply = (int[]){0};                                  \
    type **list_for_apply = (type*[]){__VA_ARGS__, stopper_for_apply};      \
    for(i=0; list_for_apply[i]!=stopper_for_apply; i++)                 \
        fn(list_for_apply[i]);                                              \
}

#define Free_all(...) Fn_apply(void, free, __VA_ARGS__)


#define STATIC_ASSERT(COND,MSG) static int static_assertion_##MSG[(COND)?1:-1]

// token pasting madness:
#define COMPILE_TIME_ASSERT3(X,L) STATIC_ASSERT(X,static_assertion_at_line_##L)
#define COMPILE_TIME_ASSERT2(X,L) COMPILE_TIME_ASSERT3(X,L)
#define COMPILE_TIME_ASSERT(X)    COMPILE_TIME_ASSERT2(X,__LINE__)

#endif //POINT_CBASE_FNUTIL_H