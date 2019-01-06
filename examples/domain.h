//
// Created by pchen on 1/5/19.
//

#ifndef POINT_CBASE_DOMAIN_H
#define POINT_CBASE_DOMAIN_H

typedef struct Domain Domain;

typedef struct Entity Entity;

typedef struct Relationship{
    Entity* from;
    Entity* to;
    void* values;
}Relationship;

//////////////////////////////////////////////////////////////////////////////
//// DOMAIN

Domain* Domain_new(const char* name);

void Domain_delete(Domain**);

const char* Domain_name(Domain* domain);

// ---------------------------------------------------------------------------
// Entity Manipulators
Entity* Domain_addEntity(Domain* domain, int type, const char* key);

Entity* Domain_findEntity(Domain* domain, int type, const char* key);

void Domain_removeEntity(Domain* domain, Entity* entity, void apply(Domain*, Entity*, void* cl), void* cl);

// ---------------------------------------------------------------------------
// Relationship Manipulators
Relationship* Domain_addRelationship(Domain* domain, Entity* from, Entity* to,
        void apply(Relationship*, void* values), void* values);

Relationship* Domain_findRelationship(Domain* domain, Entity* from, Entity* to);

void Domain_removeRelationship(Domain* domain, Relationship* relationship,
        void apply(Domain*, Relationship*, void* cl), void* cl);
    // customer application know how to deal with Relationship.values,
    // customer application can provide 'apply' callback to handle delete detail work for Relationship.values


//////////////////////////////////////////////////////////////////////////////
//// Entity

void Entity_setAttribute(Entity* entity, const char* attributeName, void* value);

void* Entity_getAttribute(Entity* entity, const char* attributeName);

#endif //POINT_CBASE_DOMAIN_H
