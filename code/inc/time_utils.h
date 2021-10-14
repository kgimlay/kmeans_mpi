// Author: Kevin Imlay

#ifndef TIME_UTILS_H
#define TIME_UTILS_H


#include <mpi.h>

// Timing data structure
typedef struct {
  double progStartTime;
  double progEndTime;
  double algoStartTime;
  double algoEndTime;
} TimeData_t;


double wallTime();
double deltaTime(double timeStart, double timeEnd);


#endif
