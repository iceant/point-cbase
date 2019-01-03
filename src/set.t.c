//
// Created by PCHEN on 2019/1/3.
//

#include "set.h"
#include "atom.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void Set_eachReleaseInt(const void* value_p, void * cl){
    printf("free(%s)\n", *(char*)value_p);
    free((void*)value_p);
}

void Set_print(const void* value_p, void* cl){
    printf(" [%s]", (char*)value_p);
}

int main(int argc, char** argv){
    printf("Set_new(0, NULL, NULL)...\n");
    Set * set = Set_new(0, NULL, NULL);
    
    int length = Set_length(set);
    assert(length==0);
    
    int i=0;
    void * remove_p = NULL;
    for(i=0; i<10; i++){
        const char* p = Atom_int(i);
        if(i==3) remove_p = (void*)p;
        Set_put(set, p);
    }
    length = Set_length(set);
    assert(length==10);
    
    printf("Set_remove(...)\n");
    void* removed_p = Set_remove(set, remove_p);
    printf("\tremoved value:%s\n", (char*)removed_p);
    
    length = Set_length(set);
    assert(length==9);
    
    printf("Set* set2 = [20...30]\n");
    Set* set2 = Set_new(0, NULL, NULL);
    for(i=20; i<30; i++){
        Set_put(set2, Atom_int(i));
    }
    assert(Set_length(set2)==10);
    
    printf("set3 = Set_union(set, set2)...\n");
    Set * set3 = Set_union(set, set2);
    Set_each(set3, Set_print, NULL);
    printf("\n");
    Set_delete(&set3);
    
    printf("Create set4...\n");
    Set * set4 = Set_new(0, NULL, NULL);
    for(i=0; i<10; i+=2){
        Set_put(set4, Atom_int(i));
    }
    assert(Set_length(set4)==5);
    printf("Values in set4...\n");
    Set_each(set4, Set_print, NULL);
    printf("\n");
    
    printf("Values in set...\n");
    Set_each(set, Set_print, NULL);
    printf("\n");
    
    printf("set5 = Set_minus(set, set4)...\n");
    Set * set5 = Set_minus(set, set4);
    Set_each(set5, Set_print, NULL);
    printf("\n");
    Set_delete(&set5);
    
    printf("set5 = Set_minus(set4, set)...\n");
    set5 = Set_minus(set4, set);
    Set_each(set5, Set_print, NULL);
    printf("\n");
    Set_delete(&set5);
    
    printf("set6 = Set_inter(set, set4)...\n");
    Set * set6 = Set_inter(set, set4);
    Set_each(set6, Set_print, NULL);
    printf("\n");
    Set_delete(&set6);
    
    printf("set6 = Set_inter(set4, set)...\n");
    set6 = Set_inter(set4, set);
    Set_each(set6, Set_print, NULL);
    printf("\n");
    Set_delete(&set6);
    
    printf("set7 = Set_diff(set, set4)...\n");
    Set * set7 = Set_diff(set, set4);
    Set_each(set7, Set_print, NULL);
    printf("\n");
    Set_delete(&set7);
    
    printf("set8 = Set_diff(set4, set)...\n");
    Set * set8 = Set_diff(set4, set);
    Set_each(set8, Set_print, NULL);
    printf("\n");
    Set_delete(&set8);
    
    void** array = Set_toArray(set2, NULL);
    void* p = NULL;
    printf("Set_toArray(set2)...\n");
    for(i=0;array[i];i++){
        printf("\t [%d] %s\n", i, (char*)array[i]);
    }
    free(array); array = NULL;/* REMEMBER TO FREE ARRAY!!! */
    
    printf("release set4...\n");
    Set_delete(&set4);
    
    printf("release set2...\n");
    Set_delete(&set2);
    
    printf("release set...\n");
    Set_delete(&set);
    
    assert(set==NULL);
}