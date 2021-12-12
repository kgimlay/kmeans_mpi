// Author: Kevin Imlay

#ifndef MPI_OMP_YINYANG_H
#define MPI_OMP_YINYANG_H


#include "mpi_yinyang.h"


// function prototypes
void run_mpi_omp_yin(PointData_t *pointList, CentroidData_t *centrList,
                  int numGroups, int maxIter, int mpi_numProc, int mpi_rank);

#endif
