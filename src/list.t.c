//
// Created by pchen on 1/1/19.
//

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void List_print(void** x, void* cl){
    char** str = (char** )x;
    printf("List_print(%s)\n", *str);
}

int compare(const void* x, const void* y){
    return strcmp(*(char**) x, *(char**)y);
}

int main(int argc, char** argv){
    int i=0;

    List* l1 = List_new(NULL);
    List* l2 = List_new("Atom", "Mem", NULL);
    printf("l2 = List_new(\"Atom\", \"Mem\", NULL)\n");

    List* l3 = List_new("Arena", "List", NULL);
    printf("l3 = List_new(\"Arena\", \"List\", NULL)\n");

    printf("List_new(NULL).length=%d\n", List_length(l1));

    printf("List_append(l2, l3)\n");
    List_append(l2, l3);
    printf("l2.length=%d\n", List_length(l2));

    l2 = List_push(l2, "New");
    printf("l2.push('New')\n");
    printf("l2.length=%d\n", List_length(l2));

    printf("-------------------------------\n");
    printf("List_each(l2, List_print, NULL)\n");
    printf("-------------------------------\n");
    List_each(l2, List_print, NULL);

    printf("-------------------------------\n");
    printf("List_toArray(l2, NULL)\n");
    printf("sort array by value\n");
    printf("-------------------------------\n");
    char** array = (char**)List_toArray(l2, NULL);
    qsort((void**)array, List_length(l2), sizeof(*array), compare);
    for(i = 0; array[i]; i++){
        printf("%s\n", array[i]);
    }
    free(array);
    array = NULL;

    printf("-------------------------------\n");
    printf("l2.reverse\n");
    l2 = List_reverse(l2);



    int len = List_length(l2);
    char* buf[10];
    printf("-------------------------------\n");
    printf("List_pop(l2,)\n");
    printf("-------------------------------\n");

    for(i=0; i<len; i++){
        l2 = List_pop(l2, &buf);
        printf("[%d]%s\n",i, *buf);
    }



    List_delete(&l1);
    List_delete(&l2);


    return 0;
}