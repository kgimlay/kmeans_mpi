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


// N-Dimensional Centroid (Centroid)
// A centroid has a dimension and its coordinates
// Note that a centroid (cluster) does not need any points assiciated with it
typedef struct NDim_Centroid {
  int id;               // cluster ID
  int dim;              // dimensionality of the data
  double *coords;       // center of the cluster
  double *prevCoords;   // previous center of the cluster
  double drift;         // distance between the current and previous coords
  int size;             // number of points in membership with cluster
  int groupId;          // group membership
} Centroid;


// N_Dimensional Data Point (Point)
// A point has a dimension, it's coordinates, and a centroid it's associated
// with. Note that a point always belongs to a centroid (cluster)
typedef struct NDim_Data_Point {
  int id;               // point id
  int dim;              // dimensionality of the data
  double *coords;       // coordinates of the data point
  Centroid *centroid;   // cluster membership
  double lb;            // lower bound (for yinyang)
  double ub;            // upper bound (for yinyang)
} Point;


// Centroid structure
// stores the centroid coords in a two dimensional array (allocated contiguously)
// stores the previous coords in a two diemnsional array (allocated contiguously)
// stores the dimension of the centroids (same for all)
// stores a list of the sizes (number of points in each centroid's group)
// stores the group ID (yinyang)
// stores the max drift (yinyang)
typedef struct {
  int k;
  int dim;
  int *groupID;             // needs to be allocated to size k
  int *sizes;               // needs to be allocated to size k
  double *coords;           // needs to be allocated to size k*dim
  double *prevCoords;       // needs to be allocated to size k*dim
  double *maxDrift;         // needs to be allocated to size k
} CentroidData_t;


// Datapoint structure
// stores the data point coords in a two dimensional array (allocated contiguously)
// stores list of corresponding centroid IDs (index of centroid list)
// stores dimension of data points (same for all)
// stores an upper and lower bound for each point (for yinyang)
// point ID is implicit as row index of coords
typedef struct {
  int n;
  int dim;
  int *centroid;            // needs to be allocated to size n
  double *coords;           // needs to be allocated to size n*dim
  double *lb;               // needs to be allocated to size n
  double *ub;               // needs to be allocated to size n
} PointData_t;


/* ----- function prototypes ----- */


void makePoints(PointData_t *pointStruct, int n, int dim);
void makeCentroids(CentroidData_t *centroidStruct, int k, int dim);
void fillPoints(double **data, int size, int dim, Point *pointList);
void freePoints(PointData_t *pointList, int n);
void freeCentroids(CentroidData_t *centroidList, int k);
void freeDataset(double **data, int num);
double calcSquaredEuclideanDist(Point point, Centroid centroid);
void primeCentroid(Centroid *centroidList, int listSize);
bool checkConvergence(Centroid *centrList, int centrList_size);
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
