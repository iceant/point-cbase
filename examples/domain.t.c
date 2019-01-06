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


int main(int argc, char** argv){
    Domain* domain = Domain_new("maze");

    printf("New Domain:%s\n", Domain_name(domain));

    Domain_addEntity(domain, MAZE_BLOCK, "0,0");
    Domain_addEntity(domain, MAZE_BLOCK, "1,0");
    Domain_addEntity(domain, MAZE_BLOCK, "2,0");
    Domain_addEntity(domain, MAZE_BLOCK, "3,0");
    Domain_addEntity(domain, MAZE_BLOCK, "4,0");
    Domain_addEntity(domain, MAZE_BLOCK, "5,0");
    Domain_addEntity(domain, MAZE_BLOCK, "6,0");
    Domain_addEntity(domain, MAZE_BLOCK, "7,0");
    Domain_addEntity(domain, MAZE_BLOCK, "8,0");
    Domain_addEntity(domain, MAZE_BLOCK, "9,0");

    Domain_addEntity(domain, MAZE_BLOCK, "0,1");
    Domain_addEntity(domain, MAZE_BLOCK, "3,1");
    Domain_addEntity(domain, MAZE_BLOCK, "7,1");
    Domain_addEntity(domain, MAZE_BLOCK, "9,1");

    Domain_addEntity(domain, MAZE_BLOCK, "0,2");
//    Domain_addEntity(domain, MAZE_BLOCK, "2,2");
    Domain_addEntity(domain, MAZE_BLOCK, "3,2");
    Domain_addEntity(domain, MAZE_BLOCK, "7,2");
    Domain_addEntity(domain, MAZE_BLOCK, "9,2");

    Domain_addEntity(domain, MAZE_BLOCK, "0,3");
    Domain_addEntity(domain, MAZE_BLOCK, "5,3");
    Domain_addEntity(domain, MAZE_BLOCK, "6,3");
    Domain_addEntity(domain, MAZE_BLOCK, "9,3");

    Domain_addEntity(domain, MAZE_BLOCK, "0,4");
    Domain_addEntity(domain, MAZE_BLOCK, "2,4");
    Domain_addEntity(domain, MAZE_BLOCK, "3,4");
    Domain_addEntity(domain, MAZE_BLOCK, "4,4");
    Domain_addEntity(domain, MAZE_BLOCK, "9,4");

    Domain_addEntity(domain, MAZE_BLOCK, "0,5");
    Domain_addEntity(domain, MAZE_BLOCK, "4,5");
    Domain_addEntity(domain, MAZE_BLOCK, "9,5");

    Domain_addEntity(domain, MAZE_BLOCK, "0,6");
    Domain_addEntity(domain, MAZE_BLOCK, "2,6");
    Domain_addEntity(domain, MAZE_BLOCK, "6,6");
    Domain_addEntity(domain, MAZE_BLOCK, "9,6");

    Domain_addEntity(domain, MAZE_BLOCK, "0,7");
    Domain_addEntity(domain, MAZE_BLOCK, "2,7");
    Domain_addEntity(domain, MAZE_BLOCK, "3,7");
    Domain_addEntity(domain, MAZE_BLOCK, "4,7");
    Domain_addEntity(domain, MAZE_BLOCK, "6,7");
    Domain_addEntity(domain, MAZE_BLOCK, "7,7");
    Domain_addEntity(domain, MAZE_BLOCK, "9,7");

    Domain_addEntity(domain, MAZE_BLOCK, "0,8");
    Domain_addEntity(domain, MAZE_BLOCK, "1,8");
    Domain_addEntity(domain, MAZE_BLOCK, "9,8");

    Domain_addEntity(domain, MAZE_BLOCK, "0,9");
    Domain_addEntity(domain, MAZE_BLOCK, "1,9");
    Domain_addEntity(domain, MAZE_BLOCK, "2,9");
    Domain_addEntity(domain, MAZE_BLOCK, "3,9");
    Domain_addEntity(domain, MAZE_BLOCK, "4,9");
    Domain_addEntity(domain, MAZE_BLOCK, "5,9");
    Domain_addEntity(domain, MAZE_BLOCK, "6,9");
    Domain_addEntity(domain, MAZE_BLOCK, "7,9");
    Domain_addEntity(domain, MAZE_BLOCK, "8,9");
    Domain_addEntity(domain, MAZE_BLOCK, "9,9");

    Point start = {1, 1};
    Point exit = {8, 8};

    Stack* stack = Stack_new();

    findPath(&start, &exit, stack, domain);

    do{
        Step* step = Stack_pop(stack);
        printf("Step(%d)[%d, %d] -> %s\n", step->order, step->point.x, step->point.y, step->direction==RIGHT?"RIGHT":
        (step->direction==DOWN?"DOWN":
        (step->direction==LEFT?"LEFT":"UP")));
        free(step);
    }while(!Stack_isEmpty(stack));

    Atom_reset();
    Stack_delete(&stack);
    Domain_delete(&domain);
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

    printf("checking pass(%d, %d)?%s\n", x, y, ret==1?"TRUE":"FALSE");

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
                    printf("change step(%d)[%d, %d] dir %d to %d\n", step->order, step->point.x, step->point.y, step->direction, step->direction+1);
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
                    printf("curpos[%d, %d]\n", curpos->x, curpos->y);
                }
            }
        }
    }while(!Stack_isEmpty(stack));
}