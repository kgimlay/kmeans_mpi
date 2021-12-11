// Author: Kevin Imlay

#ifndef MPI_OMP_LLOYD_H
#define MPI_OMP_LLOYD_H


#include "mpi_omp_lloyd.h"


// function prototypes
void run_mpi_omp_lloyd(PointData_t *pointList, CentroidData_t *centrList, int maxIter,
                  int mpi_numProc, int mpi_rank);


#endif
