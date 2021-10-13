// Author: Kevin Imlay

#ifndef MPI_LLOYD_H
#define MPI_LLOYD_H


#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <mpi.h>
#include "parameters.h"
#include "data_utils.h"


// function prototypes
void run_mpi_lloyd(PointData_t *pointList, CentroidData_t *centrList, int maxIter,
                  int mpi_numProc, int mpi_rank);


#endif
