//
// Created by pchen on 12/30/18.
//

#ifndef POINT_CBASE_PTRBAG_H
#define POINT_CBASE_PTRBAG_H

//////////////////////////////////////////////////////////////////////////////
//// TYPES
typedef void (*PtrBag_ReleaseFn)(void*);

typedef struct PtrBag PtrBag;
typedef struct PtrBagIter PtrBagIter;
typedef struct PtrBagManip PtrBagManip;

//////////////////////////////////////////////////////////////////////////////
//// PtrBagIter
PtrBagIter* PtrBagIter_new(PtrBag*);
void PtrBagIter_delete(PtrBagIter**);
int PtrBagIter_hasNext(PtrBagIter*);
void PtrBagIter_next(PtrBagIter*);
void* PtrBagIter_get(PtrBagIter*);

//////////////////////////////////////////////////////////////////////////////
//// PtrBagManip
PtrBagManip* PtrBagManip_new(PtrBag*);
void PtrBagManip_delete(PtrBagManip**);
int PtrBagManip_hasNext(PtrBagManip*);
void PtrBagManip_next(PtrBagManip*);
void* PtrBagManip_get(PtrBagManip*);
void PtrBagManip_remove(PtrBagManip*);

//////////////////////////////////////////////////////////////////////////////
//// PtrBag
PtrBag* PtrBag_new();
void PtrBag_delete(PtrBag**);

void PtrBag_add(PtrBag*, void* pointer, PtrBag_ReleaseFn releaseFn);
void PtrBag_remove(PtrBag*, const void* ptr);


#endif //POINT_CBASE_PTRBAG_H
