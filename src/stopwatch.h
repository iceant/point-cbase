//
// Created by pchen on 12/31/18.
//

#ifndef POINT_CBASE_STOPWATCH_H
#define POINT_CBASE_STOPWATCH_H

typedef struct Stopwatch Stopwatch;

Stopwatch* Stopwatch_new();

void Stopwatch_delete(Stopwatch**);

void Stopwatch_start(Stopwatch*);
void Stopwatch_stop(Stopwatch*);

double Stopwatch_elapsedUS(Stopwatch*);
double Stopwatch_elapsedMS(Stopwatch*);

#endif //POINT_CBASE_STOPWATCH_H
