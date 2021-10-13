// Author: Kevin Imlay

#ifndef LIN_LLOYD_H
#define LIN_LLOYD_H


#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "parameters.h"
#include "data_utils.h"


// function prototypes
void run_seq_lloyd(PointData_t *pointList, CentroidData_t *centrList, int maxIter);


#endif
