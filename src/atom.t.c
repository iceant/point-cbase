//
// Created by PCHEN on 2019/1/3.
//

#include "atom.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#if defined(LLONG_MAX) && !defined(LONG_LONG_MAX)
#define LONG_LONG_MAX LLONG_MAX
#endif

int main(int argc, char** argv){
    
    long intVal = INT_MAX;
    const char* aIntAtom = Atom_int(intVal);
    printf("Atom_int(%ld)=%s\n", intVal, aIntAtom);
    
    long long int64Val = LONG_LONG_MAX;
    const char* aInt64Atom = Atom_int64(int64Val);
    printf("Atom_int64(%lld)=%s\n", int64Val, aInt64Atom);
    
    const char* strAtom = Atom_string("STRING_ATOM");
    printf("Atom_string(\"STRING_ATOM\")=%s\n", strAtom);
    
    const char* strNewAtom = Atom_new("NEW_STRING_ATOM", 15);
    printf("Atom_new(\"NEW_STRING_ATOM\", 15)=[%s]\n", strNewAtom);
    
    const char* strAtom2 = Atom_string("STRING_ATOM");
    printf("strAtom==strAtom2?%s\n", strAtom2==strAtom?"TRUE":"FALSE");

    Atom_vload("ATOM_1", "2_Atom", "Hello", "Jason", "Monkey", NULL);
    const char* vatom1 = Atom_string("Monkey");
    printf("vatom1:%s\n", vatom1);

    Atom_aload((const char*[]){"A_001", "A_002", "A_003", NULL});
    printf("Atom_aload('A_002')=%s\n", Atom_string("A_002"));

    Atom_free("A_001");

    Atom_reset();

    Atom_vload("A1", "A2", "A3", NULL);
    Atom_reset();
}