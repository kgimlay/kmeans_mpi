// Author: Kevin Imlay

#ifndef MPI_UTILS_H
#define MPI_UTILS_H


#include <mpi.h>


/* ----- function prototypes ----- */

int calcPointSublistSize_rank0(int data_size, int num_ranks);
int calcPointSublistSize_rankNon0(int data_size, int num_ranks);


# endif
