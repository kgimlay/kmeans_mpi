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
void run_yinyang_firstItr(PointData_t *pointList, CentroidData_t *centroidList,
                          int numGroups)
{
    // operation variables
    double tempMinDist;
    double tempDist;
    int tempCentr = -1;

    // loop over each point
    for(int pointIdx = pointList->sublistOffset; pointIdx < pointList->n; pointIdx++)
    {
      tempMinDist = INFINITY;

      // loop over each centroid for distance calculation
      for(int centrIdx = 0; centrIdx < centroidList->k; centrIdx++)
      {
        // calculate distance to each centroid
        tempDist = calcSquaredEuclideanDist(pointList, pointIdx, centroidList, centrIdx);

        // store current minimum
        if(tempDist < tempMinDist)
        {
          tempCentr = centrIdx;
          tempMinDist = tempDist;
        }
      } /* end for */

      // update cluster membership
      pointList->centroids[pointIdx] = tempCentr;

      // set upper bound
      pointList->ub[pointIdx] = tempMinDist;

      // set lower bounds
      // loop over each group and get distance to closest centroid of each,
      // but next closest when evaluating group of current closest centroid
      for (int groupIdx = 0; groupIdx < numGroups; groupIdx++)
      {
        tempMinDist = INFINITY;

        // loop over each centroid
        for (int centrIdx = 0; centrIdx < centroidList->k; centrIdx++)
        {
          // pick only centroids belonging to group being evaluated
          // ignore the closest centroid if it comes up
          if (centroidList->groupID[centrIdx] == groupIdx
              && centrIdx != pointList->centroids[pointIdx])
          {
            // calculate distance to each centroid
            tempDist = calcSquaredEuclideanDist(pointList, pointIdx, centroidList, centrIdx);

            // store current minimum
            if(tempDist < tempMinDist)
            {
              tempMinDist = tempDist;
            }
          }
        }

        // set lower bound
        pointList->lb[pointIdx * numGroups + groupIdx] = tempMinDist;
      }

    } /* end for */
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

  // run 1 iteration of Lloyd's for initial cluster assignments
  run_yinyang_firstItr(pointList, centrList, numGroups);
  // run_seq_lloyd(pointList, centrList, 1);

  // now run Yinyang iterations
  for (int iterationCntr = 1; iterationCntr < maxIter; iterationCntr++)
  {
    // prime for next iteration
    primeCentroid(centrList);
    for(int groupIdx = 0; groupIdx < numGroups; groupIdx++)
    {
      maxDriftArr[groupIdx] = 0.0;
    }

    // update step
    updateCentroids_yinyang(centrList, pointList, maxDriftArr, numGroups);




    // filtering
    for (int pointIdx = 0; pointIdx < pointList->n; pointIdx++)
    {
      
    }





    // check for convergence
    if (checkConvergence(centrList))
    {
      // printf("Iterations: %d\n", iterationCntr+1);
      break;
    }
  }

  // recalculate center of clusters
  // updateCentroids(centrList, pointList);
  // updateCentroids_yinyang(centrList, pointList, maxDriftArr, numGroups);
}
