//
// Created by pchen on 1/5/19.
//

#include "domain.h"
#include <stdlib.h>
#include <assert.h>
#include <ptrbag.h>
#include <table.h>
#include <string.h>
#include <atom.h>

#define FREE(p)\
    free((p));(p)=NULL;

struct Domain{
    PtrBag * d_entities_p;
    PtrBag * d_relationships_p;
    char*    d_name_p;
};

struct Entity{
    int      d_type;
    PtrBag * d_relationships_p;
    Table*   d_attributes_p;
};

static void Entity_delete(Entity* entity){
    assert(entity);

    if(entity->d_relationships_p)
        PtrBag_delete(&entity->d_relationships_p);
    if(entity->d_attributes_p)
        Table_delete(&entity->d_attributes_p);
    FREE(entity);
}

static void Relationship_delete(Relationship* relationship){
    assert(relationship);
    FREE(relationship);
}

Domain *Domain_new(const char *name) {
    assert(name);
    size_t len = strlen(name);
    Domain* p = malloc(sizeof(*p) + len);
    assert(p);
    p->d_name_p = (char*)(p+1);
    memcpy(p->d_name_p, name, len);
    p->d_name_p[len] = '\0';
    p->d_entities_p = PtrBag_new();
    p->d_relationships_p = PtrBag_new();
    return p;
}

const char* Domain_name(Domain* domain){
    assert(domain);
    return domain->d_name_p;
}

void Domain_delete(Domain** pDomain){
    assert(pDomain);
    assert(*pDomain);

    struct Domain* domain = *pDomain;
    if(domain->d_relationships_p){
        PtrBag_delete(&domain->d_relationships_p);
    }
    if(domain->d_entities_p){
        PtrBag_delete(&domain->d_entities_p);
    }
    FREE(*pDomain);
}

Entity *Domain_addEntity(Domain *domain, int type, const char *key) {
    assert(domain);
    assert(key);

    size_t keyLength = strlen(key);
    struct Entity * entity = malloc(sizeof(*entity) + keyLength);
    assert(entity);

    char* cpyKey = (char*)(entity + 1);
    memcpy(cpyKey, key, keyLength);

    entity->d_type = type;
    entity->d_relationships_p = PtrBag_new();
    entity->d_attributes_p = Table_new(0, NULL, NULL);

    Table_put(entity->d_attributes_p, Atom_string("__key__"), cpyKey);

    PtrBag_add(domain->d_entities_p, entity, (void(*)(void*))Entity_delete);

    return entity;
}

Entity *Domain_findEntity(Domain *domain, int type, const char *key) {
    assert(domain);
    assert(key);

    PtrBagIter* iter = PtrBagIter_new(domain->d_entities_p);
    struct Entity* p = NULL;
    while(PtrBagIter_hasNext(iter)){
        struct Entity* entity = PtrBagIter_get(iter);
        if(entity->d_type==type){
            const char* eKey = Table_get(entity->d_attributes_p, Atom_string("__key__"));
            if(memcmp(key, eKey, strlen(key))==0){
                p = entity;
                break;
            }
        }

        PtrBagIter_next(iter);
    }

    PtrBagIter_delete(&iter);
    return p;
}

void Entity_setAttribute(Entity *entity, const char *attributeName, void *value) {
    assert(entity);
    assert(attributeName);
    if(!value) return;

    Table_put(entity->d_attributes_p, Atom_string(attributeName), value);
}

void *Entity_getAttribute(Entity *entity, const char *attributeName) {
    assert(entity);
    assert(attributeName);
    return Table_get(entity->d_attributes_p, Atom_string(attributeName));
}

Relationship *
Domain_addRelationship(Domain *domain, Entity *from, Entity *to, void (*apply)(Relationship *, void *), void *values) {
    assert(domain);
    assert(from);
    assert(to);

    Relationship* p = malloc(sizeof(*p));
    assert(p);

    PtrBag_add(domain->d_relationships_p, p, (void(*)(void*))Relationship_delete);
    p->from = from;
    p->to = to;

    if(apply){
        apply(p, values);
    }else{
        p->values = values;
    }

    return p;
}

Relationship *Domain_findRelationship(Domain *domain, Entity *from, Entity *to) {
    assert(domain);
    assert(from);
    assert(to);
    Relationship* p = NULL;
    PtrBagIter * iter = PtrBagIter_new(domain->d_relationships_p);
    while(PtrBagIter_hasNext(iter)){
        Relationship * relationship = PtrBagIter_get(iter);
        if(relationship->from==from && relationship->to==to /*need compare function here*/){
            p = relationship;
            break;
        }
        PtrBagIter_next(iter);
    }

    PtrBagIter_delete(&iter);
    return p;
}

void
Domain_removeRelationship(Domain *domain, Relationship *relationship, void (*apply)(Domain *, Relationship *, void *),
                          void *cl) {
    assert(domain);
    assert(relationship);

    if(apply){
        apply(domain, relationship, cl);
    }

    PtrBag_remove(domain->d_relationships_p, relationship);
}

void Domain_removeEntity(Domain *domain, Entity *entity, void (*apply)(Domain *, Entity *, void *), void *cl) {
    assert(domain);
    assert(entity);

    PtrBagManip * manip = PtrBagManip_new(entity->d_relationships_p);
    while(PtrBagManip_hasNext(manip)){
        Relationship* relationship = PtrBagManip_get(manip);
        PtrBag_remove(domain->d_relationships_p, relationship);
        PtrBagManip_remove(manip);
    }
    PtrBagManip_delete(&manip);
    if(apply){
        apply(domain, entity, cl);
    }
    PtrBag_remove(domain->d_entities_p, entity);
}




