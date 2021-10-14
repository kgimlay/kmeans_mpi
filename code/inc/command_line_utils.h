// Author: Kevin Imlay

#ifndef COMMAND_LINE_UTILS_H
#define COMMAND_LINE_UTILS_H


#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "parameters.h"
#include "utils.h"
#include "data_utils.h"


/* ----- Definitions ------ */


#define NUM_REQ_CMD_ARGS 5
#define NUM_OPT_CMD_ARGS 2

#define MAX_ITR_FLAG "-i"
#define OUTPUT_FLAG "-s"
#define OUTPUT_TIME "t"
#define OUTPUT_CENT "c"
#define OUTPUT_PONT "p"


/* ----- funciton prototypes ----- */


bool parse_commandline(int argc, char *argv[], int *dataSize, int *dataDim,
  int *numClust, char *datasetFilePath, int *itrMax,
  SaveOptions_t *fileOutputOptions, ALGO_CODE *algo_select);


#endif
