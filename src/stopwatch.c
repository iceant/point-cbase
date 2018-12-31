//
// Created by pchen on 12/31/18.
//
#include "stopwatch.h"
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>

double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;

    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;

    diff = (double)y_ms - (double)x_ms;

    return diff;
}

struct Stopwatch{
    struct timeval d_start;
    struct timeval d_stop;
};

void Stopwatch_delete(Stopwatch ** pStopwatch) {
    assert(pStopwatch);
    assert(*pStopwatch);

    free(*pStopwatch);
    *pStopwatch = NULL;
}

Stopwatch* Stopwatch_new(){
    Stopwatch* p = malloc(sizeof(*p));
    assert(p);
    gettimeofday(&p->d_start, NULL);
    return p;
}

void Stopwatch_start(Stopwatch * stopwatch) {
    assert(stopwatch);
    gettimeofday(&stopwatch->d_start, NULL);
}

void Stopwatch_stop(Stopwatch * stopwatch) {
    assert(stopwatch);
    gettimeofday(&stopwatch->d_stop, NULL);
}

double Stopwatch_elapsedUS(Stopwatch * stopwatch) {
    return time_diff(stopwatch->d_start, stopwatch->d_stop);
}

double Stopwatch_elapsedMS(Stopwatch *stopwatch) {
    assert(stopwatch);
    return stopwatch->d_stop.tv_usec - stopwatch->d_start.tv_usec;
}












