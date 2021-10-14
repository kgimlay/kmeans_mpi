// Author: Kevin Imlay

#ifndef LINEAR_YINYANG
#define LINEAR_YINYANG


#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "parameters.h"
#include "data_utils.h"
#include "seq_lloyd.h"


// function prototypes
void run_lin_yin(PointData_t *pointList, CentroidData_t *centrList,
                  int numGroups, int maxIter);


#endif
