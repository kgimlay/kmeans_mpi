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
  int tempDist;
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
      // reset global lower
      tmpGlobLwr = INFINITY;

      // update upper bound
      pointList->ub[pointIdx] += centrList->drift[pointList->centroids[pointIdx]];

      // update lower bound
      for (int groupIdx = 0; groupIdx < numGroups; groupIdx++)
      {
        pointList->lb[pointIdx * numGroups + groupIdx] -= maxDriftArr[groupIdx];
        if (pointList->lb[pointIdx * numGroups + groupIdx] < tmpGlobLwr)
        {
          tmpGlobLwr = pointList->lb[pointIdx * numGroups + groupIdx];
        }
      }

      // global filtering
      if (tmpGlobLwr < pointList->ub[pointIdx])
      {
        // tighten upper bound
        pointList->ub[pointIdx] = calcSquaredEuclideanDist(pointList, pointIdx, centrList, pointList->centroids[pointIdx]);

        // check upper bound again
        if (tmpGlobLwr < pointList->ub[pointIdx])
        {
          // group filtering
          for (int groupIdx = 0; groupIdx < numGroups; groupIdx++)
          {
            if (pointList->lb[pointIdx * numGroups + groupIdx] < pointList->ub[pointIdx])
            {
              groupLclArr[pointIdx * numGroups + groupIdx] = true;
            }
            else
            {
              groupLclArr[pointIdx * numGroups + groupIdx] = false;
            }
          }

          // reset lower bounds for point
          for (int groupIdx = 0; groupIdx < numGroups; groupIdx++)
          {
            // if group is not blocked by group filter
            if (groupLclArr[pointIdx * numGroups + groupIdx])
            {
              pointList->lb[pointIdx * numGroups + groupIdx] = INFINITY;
            }
          }

          // iterate over centroids for distance calculations
          for (int centroidIdx = 0; centroidIdx < centrList->k; centroidIdx++)
          {
            // if centroid's group is marked
            if (groupLclArr[pointIdx * numGroups + centrList->groupID[pointList->centroids[pointIdx]]])
            {
              // skip if centroid assignment did not change
              if (centroidIdx == pointList->prevCentroids[pointIdx])
              {
                continue;
              }

              // compute distance between point and centroid
              tempDist = calcSquaredEuclideanDist(pointList, pointIdx, centrList, centroidIdx);

              // if less, reassign centroid
              if (tempDist < pointList->ub[pointIdx])
              {
                pointList->lb[pointIdx * numGroups + centrList->groupID[pointList->centroids[pointIdx]]] = pointList->ub[pointIdx];
                pointList->centroids[pointIdx] = centroidIdx;
                pointList->ub[pointIdx] = tempDist;
              }
              // set lower bound if less than current lower bound for group
              else if (tempDist < pointList->lb[pointIdx * numGroups + centrList->groupID[pointList->centroids[pointIdx]]])
              {
                pointList->lb[pointIdx * numGroups + centrList->groupID[pointList->centroids[pointIdx]]] = tempDist;
              }
            }
          }
        }
      }
    }





    // check for convergence
    if (checkConvergence(centrList))
    {
      printf("Iterations: %d\n", iterationCntr+1);
      break;
    }
  }

  // recalculate center of clusters
  updateCentroids_yinyang(centrList, pointList, maxDriftArr, numGroups);
}
