// Author: Kevin Imlay

#ifndef PARAMETERS_H
#define PARAMETERS_H


// enable debug printing statements
// will decrease performance, set to 0 for time critical
#define DEBUG 0

// enable mpi init and finalize
// may help improve performance when MPI is not needed
// i.e. when running sequential algorithms
#define MPI 0

// maximum number of cores to run MPI on
// take care not to exceed machine limits
#define MAX_CORES 8

// maximum number of iterations to allow
#define DEFAULT_MAX_ITERATIONS 10000

// default verbose level for printing
// 0: no print statements
// 1: print time
// 2: centroid coords and above
#define DAFAULT_VERB_LEVEL 1

// maximum size of string buffers
#define MAX_STR_BUFF_SIZE 128

// random generator seed
#define RAND_SEED 90

// select centroid start method
// 0: first N datapoints
// 1: spanning mean of alternating points
// 2: random datapoints
#define CENTR_START_METHOD 0

// valid algorithms to run
typedef enum {
  SEQ_LLOYD = 0,
  MPI_LLOYD,
  MPI_OMP_LLOYD,
  SEQ_YINYANG,
  MPI_YINYANG,
  MPI_OMP_YINYANG
} ALGO_CODE;


#endif
