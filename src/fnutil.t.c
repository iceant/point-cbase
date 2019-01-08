//
// Created by PCHEN on 2019/1/7.
//

#include "fnutil.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv){
    double* x = malloc(10);
    double* y = malloc(100);
    double* z = malloc(1000);
    
    Free_all(x, y, z);
    
    COMPILE_TIME_ASSERT(sizeof(int)==4);
    
    return 0;
}