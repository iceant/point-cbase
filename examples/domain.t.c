//
// Created by pchen on 1/5/19.
//

#include "domain.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <stack.h>
#include <fmt.h>
#include <atom.h>
#ifdef __WIN32
#include <windows.h>
#endif

#define MAZE_BLOCK 1
#define MAZE_USED 2

#define IS_EQ(p1, p2)\
    ((p1)->x==(p2)->x) && ((p1)->y==(p2)->y)

typedef enum Direction{
    RIGHT=1,
    DOWN=2,
    LEFT=3,
    UP=4
}Direction;

typedef struct Point{
    int x;
    int y;
}Point;

typedef struct Step{
    int order;
    Point point;
    int direction;
}Step;

#ifdef __WIN32
#define gotoxy(x, y)\
    do{\
        COORD coord;\
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);\
        coord.X = (x);\
        coord.Y = (y);\
        SetConsoleCursorPosition(hStdout, coord);\
    }while(0)
#else
#define gotoxy(x,y) printf("\033[%d;%dH", (x), (y))
#endif

static inline
const char* make_atom(int x, int y){
    char* str = Fmt_string("%d,%d", x, y);
    const char* ret =  Atom_string(str);
    free(str);
    return ret;
}

static inline
Step* make_step(int order, Point* point, int direction){
    assert(point);
    Step * p = malloc(sizeof(*p));
    assert(p);
    p->order = order;
    p->point.x = point->x;
    p->point.y = point->y;
    p->direction = direction;
    return p;
}

void findPath(Point *start, Point *end, Stack* stack, Domain* domain);

static const char * map =
        "XXXXXXXXXX"
        "XS X   X X"
        "X XX   X X"
        "X    XX  X"
        "X  XXX   X"
        "X    X   X"
        "X X   X  X"
        "X XXX XX X"
        "XX      EX"
        "XXXXXXXXXX";

int main(int argc, char** argv){
    Domain* domain = Domain_new("maze");

//    printf("New Domain:%s\n", Domain_name(domain));

    int i=0;
    int j = 0;
    Point start = {0, 0};
    Point exit = {0, 0};
    int idx = 0;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            idx = i*10+j;
            if(map[idx]=='X'){
                Domain_addEntity(domain, MAZE_BLOCK, make_atom(j, i));
            }
            if(map[idx]=='S'){
                start.x=j;
                start.y=i;
            }else if(map[idx]=='E'){
                exit.x = j;
                exit.y = i;
            }
        }
        printf("\n");
    }

    if(start.x==0 && start.y==0) return 0;
    
    Stack* stack = Stack_new();

    findPath(&start, &exit, stack, domain);

    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            idx = i*10+j;
            gotoxy(j, i);
            printf("%c",map[idx]);
        }
        printf("\n");
    }
    
    do{
        Step* step = Stack_pop(stack);
//        printf("Step(%d)[%d, %d] -> %s\n", step->order, step->point.x, step->point.y, step->direction==RIGHT?"RIGHT":
//        (step->direction==DOWN?"DOWN":
//        (step->direction==LEFT?"LEFT":"UP")));
        
        gotoxy(step->point.x, step->point.y);
        if(step->point.x==exit.x && step->point.y == exit.y){
            printf("$", step->order);
        }else if(step->direction==RIGHT)
            printf(">", step->order);
        else if(step->direction==DOWN){
            printf("|", step->order);
        }else if(step->direction==LEFT){
            printf("<", step->order);
        }else if(step->direction==UP){
            printf("^", step->order);
        }
        
        free(step);
    }while(!Stack_isEmpty(stack));

    Atom_reset();
    Stack_delete(&stack);
    Domain_delete(&domain);
    
    return 0;
}

int is_canpass(Domain *domain, int x, int y){
    assert(x>=0 && y>=0);
    const char* atom = make_atom(x, y);
    int ret = 1;
    if(Domain_findEntity(domain, MAZE_BLOCK, atom))
        ret = 0;
    else if(Domain_findEntity(domain, MAZE_USED, atom))
        ret = 0;
    else ret=1;

//    printf("checking pass(%d, %d)?%s\n", x, y, ret==1?"TRUE":"FALSE");

    return ret;
}

void mark_used(Domain* domain, int x, int y){
    Domain_addEntity(domain, MAZE_USED, make_atom(x, y));
}

void findPath(Point* start, Point* end, Stack* stack, Domain* domain){
    assert(start);
    assert(end);
    assert(stack);
    assert(domain);

    Point* curpos = start;
    int curstep = 0;

    do{
        if(is_canpass(domain, curpos->x, curpos->y)){
            mark_used(domain, curpos->x, curpos->y);
            Stack_push(stack, make_step(++curstep, curpos, RIGHT));
            if(IS_EQ(curpos, end)){
                return;
            }
            curpos->x+=1; // move to rigth
        }else{
            if(!Stack_isEmpty(stack)){
                Step* step = Stack_pop(stack);
                while(step->direction==4 && !Stack_isEmpty(stack)){
                    mark_used(domain, step->point.x, step->point.y);
                    free(step);
                    step = Stack_pop(stack);
                }
                if(step->direction<4){
//                    printf("change step(%d)[%d, %d] dir %d to %d\n", step->order, step->point.x, step->point.y, step->direction, step->direction+1);
                    step->direction++; // change direction
                    Stack_push(stack, step);
                    curpos->x = step->point.x;
                    curpos->y = step->point.y;

                    if(step->direction==RIGHT){
                        curpos->x+=1;
                    }else if(step->direction==DOWN){
                        curpos->y+=1;
                    }else if(step->direction==LEFT){
                        curpos->x = (curpos->x-1)<0?0:(curpos->x-1);
                    }else if(step->direction==UP){
                        curpos->y-=1;
                        if(curpos->y<0) curpos->y=0;
                    }
//                    printf("curpos[%d, %d]\n", curpos->x, curpos->y);
                }
            }
        }
    }while(!Stack_isEmpty(stack));
}