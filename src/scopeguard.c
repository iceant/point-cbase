//
// Created by pchen on 12/30/18.
//

#include "scopeguard.h"
#include <stdlib.h> // malloc
#include <assert.h>

#define FALSE   0
#define TRUE    1

typedef struct ScopeGuard_Item{
    struct ScopeGuard_Item* d_next_p;
    ScopeGuard_ReleaseFn d_fn_p;
    void* d_pointer_p;
}ScopeGuard_Item;

typedef struct ScopeGuard{
    struct ScopeGuard_Item* d_list_p;
    int d_isDismiss;
}ScopeGuard;

ScopeGuard* ScopeGuard_new(){
    ScopeGuard* p = malloc(sizeof(*p));
    assert(p);
    p->d_list_p = NULL;
    p->d_isDismiss = FALSE;
    return p;
}

void ScopeGuard_onExit(ScopeGuard* scopeGuard_p, ScopeGuard_ReleaseFn fn, void* resource_p){
    assert(scopeGuard_p);
    assert(fn);
    ScopeGuard_Item * p = malloc(sizeof(*p));
    assert(p);
    p->d_next_p = scopeGuard_p->d_list_p;
    p->d_fn_p = fn;
    p->d_pointer_p = resource_p;
    scopeGuard_p->d_list_p = p;
}

void ScopeGuard_exit(ScopeGuard** scopeGuard){
    assert(scopeGuard);
    assert(*scopeGuard);

    ScopeGuard * scopeGuard_p = *scopeGuard;
    ScopeGuard_Item* p;
    if(scopeGuard_p->d_isDismiss==FALSE){
        while(scopeGuard_p->d_list_p){
            p = scopeGuard_p->d_list_p;
            p->d_fn_p(p->d_pointer_p);
            scopeGuard_p->d_list_p = p->d_next_p;
            free(p);
        }
    }else{
        while(scopeGuard_p->d_list_p){
            p = scopeGuard_p->d_list_p;
            scopeGuard_p->d_list_p = p->d_next_p;
            free(p);
        }
    }
    free(scopeGuard_p);
    *scopeGuard = NULL;
}

void ScopeGuard_dismiss(ScopeGuard * scopeGuard_p) {
    scopeGuard_p->d_isDismiss = TRUE;
}

int ScopeGuard_isDismissed(ScopeGuard * scopeGuard_p) {
    return scopeGuard_p->d_isDismiss==TRUE;
}
