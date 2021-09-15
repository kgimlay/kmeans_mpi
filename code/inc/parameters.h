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

// valid algorithms to run
typedef enum {
  LINEAR_LLOYD = 0
} ALGO_CODE;


#endif
