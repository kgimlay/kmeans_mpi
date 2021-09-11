// Author: Kevin Imlay

#ifndef LIN_LLOYD_H
#define LIN_LLOYD_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "parameters.h"


// typedefs

// N-Dimensional Centroid (Centroid)
// A centroid has a dimension and its coordinates
// Note that a centroid (cluster) does not need any points assiciated with it
typedef struct NDim_Centroid {
  int id;               // cluster ID
  int dim;              // dimensionality of the data
  double *coords;       // center of the cluster
  double *prevCoords;   // previous center of the cluster
  int size;             // number of points in membership with cluster
} Centroid;


// N_Dimensional Data Point (Point)
// A point has a dimension, it's coordinates, and a centroid it's associated
// with. Note that a point always belongs to a centroid (cluster)
typedef struct NDim_Data_Point {
  int id;               // point id
  int dim;              // dimensionality of the data
  double *coords;       // coordinates of the data point
  Centroid *centroid;   // cluster membership
} Point;


// function prototypes
void run_lin_lloyd(Point *pointList, int pointList_size, Centroid *centrList,
                    int centrList_size, int maxIter);


#endif
