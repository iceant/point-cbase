//
// Created by PCHEN on 2019/1/7.
//

#include <stdio.h>
#include <stdlib.h>

main() {
    int MB = 0;
    while (malloc(1 << 20)) ++MB;
    printf("%lld\n", 1<<20/*1024*1024*/);
    printf("Allocated %d MB total\n", MB);
    printf("Allocated %d GB total\n", MB/1024);
}