// Author: Kevin Imlay

#ifndef COMMAND_LINE_UTILS_H
#define COMMAND_LINE_UTILS_H


#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "parameters.h"
#include "utils.h"

// funciton prototypes
bool parse_commandline(int argc, char *argv[], ALGO_CODE *algo,
  char *dataFilePath_buff, int *dataSetSize, int *dataDimensionality,
  int *numClusters, int *numIterations, char *outputFilePath_buff);


#endif
