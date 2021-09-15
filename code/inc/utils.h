// Author: Kevin Imlay

#ifndef HELPER_H
#define HELPER_H


#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "parameters.h"

// function prototypes
bool str_to_algo(char *input, ALGO_CODE *algo);
bool algo_to_str(ALGO_CODE algo, char *out);
void print_operation_parameters(ALGO_CODE algorithm_code, char *filePath_str,
                                int dataSize, int dataDim, int numClusters,
                                int numCores);

#endif
