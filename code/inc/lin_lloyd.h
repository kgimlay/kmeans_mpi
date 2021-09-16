// Author: Kevin Imlay

#ifndef LIN_LLOYD_H
#define LIN_LLOYD_H


#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "parameters.h"
#include "data_utils.h"


// function prototypes
void run_lin_lloyd(Point *pointList, int pointList_size, Centroid *centrList,
                    int centrList_size, int maxIter);


#endif
