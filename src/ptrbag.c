//
// Created by pchen on 12/30/18.
//

#include "ptrbag.h"
#include <stdlib.h>
#include <assert.h>

typedef struct PtrBagLink {
    void *d_pointer_p;
    PtrBag_ReleaseFn d_releasefn_p;
    struct PtrBagLink *d_next_p;
} PtrBagLink;

#define PtrBagLink_DELETE(link_p)\
    if((link_p)->d_releasefn_p){\
      (link_p)->d_releasefn_p((link_p)->d_pointer_p);\
    }\
    free((link_p));

struct PtrBag {
    PtrBagLink * d_root_p;
};

struct PtrBagIter{
    PtrBagLink * d_link_p;
};

struct PtrBagManip{
    PtrBagLink ** d_addrLink_p;
};

//////////////////////////////////////////////////////////////////////////////
//// PtrBagManip

#define PtrBagManip_INIT(man, ptrbag_ptr)\
    (man).d_addrLink_p = &((ptrbag_ptr)->d_root_p);

PtrBagManip* PtrBagManip_new(PtrBag* ptrBag){
    assert(ptrBag);
    PtrBagManip* p = malloc(sizeof(*p));
    assert(p);
    p->d_addrLink_p = &(ptrBag->d_root_p);
    return p;
}

void PtrBagManip_delete(PtrBagManip** ptrBagManip){
    assert(ptrBagManip);
    assert(*ptrBagManip);

    free(*ptrBagManip);
    *ptrBagManip = NULL;
}

int PtrBagManip_hasNext(PtrBagManip* ptrBagManip){
    assert(ptrBagManip);
    return *(ptrBagManip->d_addrLink_p)!=NULL;
}

void* PtrBagManip_get(PtrBagManip* ptrBagManip){
    assert(ptrBagManip);
    return (*(ptrBagManip->d_addrLink_p))->d_pointer_p;
}

void PtrBagManip_remove(PtrBagManip* ptrBagManip){
    assert(ptrBagManip);
    PtrBagLink * tmp = *ptrBagManip->d_addrLink_p;
    *(ptrBagManip->d_addrLink_p) = (*ptrBagManip->d_addrLink_p)->d_next_p;
    PtrBagLink_DELETE(tmp);
}

void PtrBagManip_next(PtrBagManip* ptrBagManip){
    assert(ptrBagManip);
    *(ptrBagManip->d_addrLink_p) = (*ptrBagManip->d_addrLink_p)->d_next_p;
}

//////////////////////////////////////////////////////////////////////////////
//// PtrBag

PtrBag* PtrBag_new(){
    PtrBag * p = malloc(sizeof(*p));
    assert(p);
    p->d_root_p = NULL;
    return p;
}

void PtrBag_delete(PtrBag** ptrBag){
    assert(ptrBag);
    assert(*ptrBag);
    PtrBagManip man;
    PtrBagManip_INIT(man, *ptrBag);
    while(PtrBagManip_hasNext(&man)){
        PtrBagManip_remove(&man);
    }
    free(*ptrBag);
    *ptrBag = NULL;
}

void PtrBag_add(PtrBag* ptrBag, void *pointer, PtrBag_ReleaseFn releaseFn) {
    assert(ptrBag);
    assert(pointer);

    PtrBagLink *p = malloc(sizeof(*p));
    assert(p);
    p->d_pointer_p = pointer;
    p->d_releasefn_p = releaseFn;
    p->d_next_p = ptrBag->d_root_p;
    ptrBag->d_root_p = p;
}

void PtrBag_remove(PtrBag* ptrBag, const void* ptr){
    assert(ptrBag);
    PtrBagManip manip;
    PtrBagManip_INIT(manip, ptrBag);
    while(PtrBagManip_hasNext(&manip)){
        (PtrBagManip_get(&manip)==ptr)?PtrBagManip_remove(&manip):PtrBagManip_next(&manip);
    }
}

////////////////////////////////////////////////////////////
PtrBagIter *PtrBagIter_new(PtrBag * ptrBag) {
    assert(ptrBag);
    PtrBagIter * p = malloc(sizeof(*p));
    assert(p);
    p->d_link_p = ptrBag->d_root_p;
    return p;
}

int PtrBagIter_hasNext(PtrBagIter * ptrBagIter) {
    assert(ptrBagIter);
    return ptrBagIter->d_link_p!=NULL;
}

void PtrBagIter_next(PtrBagIter* ptrBagIter){
    assert(ptrBagIter);
    ptrBagIter->d_link_p = ptrBagIter->d_link_p->d_next_p;
}

void* PtrBagIter_get(PtrBagIter* ptrBagIter){
    assert(ptrBagIter);
    return ptrBagIter->d_link_p->d_pointer_p;
}

void PtrBagIter_delete(PtrBagIter ** ptrBagIter) {
    assert(ptrBagIter);
    assert(*ptrBagIter);
    free(*ptrBagIter);
    *ptrBagIter = NULL;
}






