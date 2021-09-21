// Author: Kevin Imlay

#ifndef DATA_UTILS_H
#define DATA_UTILS_H


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
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
void makePoints(Point *pointList, int size, int dim);
void makeCentroids(Centroid *centroidList, int num, int dim);
void fillPoints(double **data, int size, int dim, Point *pointList);
void freePoints(Point *pointList, int num);
void freeCentroids(Centroid *centroidList, int num);
void freeDataset(double **data, int num);
double calcSquaredEuclideanDist(Point point, Centroid centroid);
void primeCentroid(Centroid *centroidList, int listSize);
void updatePointClusterMembership(Point *pointList, int pointListSize,
                                  Centroid *centroidList, int centroidlistSize);
void updateCentroids(Point *pointList, int pointListSize,
                      Centroid *centroidList, int centroidlistSize);
void startCentroids(Centroid *centrList, int centrListSize,
                    Point *pointList, int pointListSize);
void updateCentroids_MPI(Point *pointSublist, int pointSublist_size,
                      Centroid *centrList, int centrList_size,
                      int mpi_rank, int mpi_numProc, double *mpiCentrDataList,
                      int mpiCentrDataList_width);

#endif
