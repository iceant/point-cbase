// stack.h                                                            -- C++ -- 
// Created by PCHEN on 2019/1/4.
// Copyright (c) 2019 POINTCX All rights reserved.
#ifndef POINT_CBASE_STACK_H
#define POINT_CBASE_STACK_H

typedef struct Stack Stack;

//////////////////////////////////////////////////////////////////////////////
//// CREATORS
Stack* Stack_new(void);

void Stack_delete(Stack** stack);
    // make sure the stack is empty before delete it.
    // that means to pop everything out and release the void* resources first.

//////////////////////////////////////////////////////////////////////////////
//// MANIPULATORS
void Stack_push(Stack* stk, void* x);

void* Stack_pop(Stack* stk);

//////////////////////////////////////////////////////////////////////////////
//// ACCESSORS
int Stack_isEmpty(Stack* stk);

#endif //POINT_CBASE_STACK_H