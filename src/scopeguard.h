//
// Created by pchen on 12/30/18.
//

#ifndef POINT_CBASE_SCOPEGUARD_H
#define POINT_CBASE_SCOPEGUARD_H

//////////////////////////////////////////////////////////////////////////////
//// TYPES
typedef void (*ScopeGuard_ReleaseFn)(void*);

typedef struct ScopeGuard ScopeGuard;

//////////////////////////////////////////////////////////////////////////////
//// CREATORS
ScopeGuard* ScopeGuard_new();
void ScopeGuard_exit(ScopeGuard**);

//////////////////////////////////////////////////////////////////////////////
//// MANIPULATORS
void ScopeGuard_onExit(ScopeGuard*, ScopeGuard_ReleaseFn, void* resource_ptr);
void ScopeGuard_dismiss(ScopeGuard*);

int ScopeGuard_isDismissed(ScopeGuard*);

#endif //POINT_CBASE_SCOPEGUARD_H
