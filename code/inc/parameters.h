// Author: Kevin Imlay

#ifndef PARAMETERS_H
#define PARAMETERS_H


// enable verbose printing of operation
// will decrease performance, set to 0 for time critical
#define VERBOSE 1

// enable debug printing statements
// will decrease performance, set to 0 for time critical
#define DEBUG 0

// maximum number of cores to run MPI on
// take care not to exceed machine limits
#define MAX_CORES 8

// maximum number of iterations to allow
#define DEFAULT_MAX_ITERATIONS 10000

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
  SEQ_YINYANG,
  MPI_YINYANG
} ALGO_CODE;


#endif
