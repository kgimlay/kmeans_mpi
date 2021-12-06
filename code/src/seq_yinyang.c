// Author: Keivn Imlay

#include "../inc/seq_yinyang.h"


/*

*/
void groupCentroids(CentroidData_t *centroids, int t)
{
  // operation variables
  int numCentroids = centroids->k;
  int dim = centroids->dim;
  PointData_t centPoints;
  CentroidData_t centCentroids;

  // store centroid data into points
  makePoints(&centPoints, numCentroids, dim, t);
  for (int i = 0; i < numCentroids; i++)
  {
    for (int j = 0; j < dim; j++)
    {
      centPoints.coords[i * dim + j] = centroids->coords[i * dim + j];
    }
  }

  // make centroids (groups)
  makeCentroids(&centCentroids, t, dim);
  startCentroids(&centCentroids, &centPoints);

  // run kmeans to group centroids for 10 iterations (max)
  run_seq_lloyd(&centPoints, &centCentroids, 10);

  // get centroid assignments as group assignments
  for (int i = 0; i < numCentroids; i++)
  {
    centroids->groupID[i] = centPoints.centroids[i];
  }

  // free temporary data
  freePoints(centPoints);
  freeCentroids(centCentroids);
}


/*

*/
void run_seq_yin(PointData_t *pointList, CentroidData_t *centrList,
                  int numGroups, int maxIter)
{
  // operation variables
  double *maxDriftArr = (double *)malloc(sizeof(double) * numGroups);
  double tmpGlobLwr = INFINITY;
  bool *groupLclArr = (bool *)malloc(sizeof(bool) * pointList->n * numGroups);

  // initiatilize to INFINITY
  for(int i = 0; i < pointList->n * numGroups; i++)
  {
    pointList->lb[i] = INFINITY;
  }

  // cluster the centroids into t groups
  startCentroids(centrList, pointList);
  groupCentroids(centrList, numGroups);

  
}
