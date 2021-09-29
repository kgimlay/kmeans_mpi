// Author: Kevin Imlay

#ifndef MPI_YINYANG_H
#define MPI_YINYANG_H


#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <mpi.h>
#include "parameters.h"
#include "data_utils.h"


// function prototypes
void run_mpi_yin(Point *pointList, int pointList_size, Centroid *centrList,
                    int centrList_size, int maxIter, int mpi_numProc, int mpi_rank);


#endif
