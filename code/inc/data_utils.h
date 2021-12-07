// Author: Kevin Imlay

#ifndef DATA_UTILS_H
#define DATA_UTILS_H


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <mpi.h>
#include "parameters.h"


/* ----- Defines ----- */





/* ----- typedefs ----- */


// N-Dimensional Centroids
// A centroid has a dimension and its coordinates
// Note that a centroid (cluster) does not need any points assiciated with it
typedef struct {
  int k;
  int dim;
  int numGroups;
  int *groupID;             // needs to be allocated to size k
  int *sizes;               // needs to be allocated to size k
  double *coords;           // needs to be allocated to size k*dim
  double *prevCoords;       // needs to be allocated to size k*dim
  double *drift;            // needs to be allocated to size k

} CentroidData_t;


// N_Dimensional Data Points
// A point has a dimension, it's coordinates, and a centroid it's associated
// with. Note that a point always belongs to a centroid (cluster)
typedef struct {
  int n;
  int dim;
  int sublistN;             // size of sublist (used for sublisting)
  int sublistOffset;        // start sublist offset for associated lists below
  int *centroids;           // needs to be allocated to size n
  int *prevCentroids;       // needs to be allocated to size n
  double *coords;           // needs to be allocated to size n*dim
  double *lb;               // needs to be allocated to size n*t
  double *ub;               // needs to be allocated to size n
} PointData_t;


// Output Options
// stores options for what informaiton to output
typedef struct {
  bool outPoints;           // output point assignemnts
  bool outCentroids;        // output centroid coordinates
  bool outTime;             // output time information
} OutputOptions_t;


// File Output Options
// Stores options for file output such as output path and what files to output
typedef struct {
  char *path;               // parent folder to output files to
  OutputOptions_t options;  // options on what to output
} SaveOptions_t;


/* ----- function prototypes ----- */


void makePoints(PointData_t *pointStruct, int n, int dim, int t);
void makeCentroids(CentroidData_t *centroidStruct, int k, int dim);
void makeSaveOptions(SaveOptions_t *saveOptions);
void freePoints(PointData_t pointList);
void freeCentroids(CentroidData_t centroidList);
void freeSaveOptions(SaveOptions_t saveOptions);
double calcSquaredEuclideanDist(PointData_t *points, int pointId,
                                CentroidData_t *centroids, int centroidId);
void primeCentroid(CentroidData_t *centroidList);
bool checkConvergence(CentroidData_t *centrList);
void assignCentroids(PointData_t *pointList,
                                  CentroidData_t *centroidList);
void updateCentroids(CentroidData_t *centrList, PointData_t *pointList);
void startCentroids(CentroidData_t *centrList, PointData_t *pointList);
void updateCentroids_MPI(PointData_t *pointList, CentroidData_t *centrList,
                      int mpi_rank, int mpi_numProc, int rank_0_sublist_size,
                      int rank_non_0_sublist_size);
void updateCentroids_yinyang(CentroidData_t *centrList, PointData_t *pointList,
                            double *maxDrift, int numGroups);

#endif
