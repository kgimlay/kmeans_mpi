// Author: Keivn Imlay

#include "../inc/seq_yinyang.h"



/*
Function used to do an intial iteration of K-means
*/
void initPoints(PointData_t *points, CentroidData_t *centroids, double *pointLwrs)
{
  unsigned int pointIdx, centrIdx;
  double currDistance;

  // start single standard k-means iteration for initial bounds and cluster assignments
  for (pointIdx = 0; pointIdx < points->n; pointIdx++)
  {
    // pointInfo[pntIndex].uprBound = INFINITY;
    points->ub[pointIdx] = INFINITY;

    // for all centroids
    for (centrIdx = 0; centrIdx < centroids->k; centrIdx++)
    {
      // currDistance is equal to the distance between the current feature
      // vector being inspected, and the current centroid being compared
      currDistance = calcSquaredEuclideanDist(points, pointIdx,
                                    centroids, centrIdx);

      // if the the currDistance is less than the current minimum distance
      if (currDistance < points->ub[pointIdx])
      {
        if (points->ub[pointIdx] != INFINITY)
        {
          // pointLwrs[(pointIdx * centroids->numGroups) + centroids->groupID[points->centroids[pointIdx]]] = points->ub[pointIdx];
        }

        // update assignment and upper bound
        // points->centroids[pointIdx] = centrIdx;
        // points->ub[pointIdx] = currDistance;
      }
      else if (currDistance < pointLwrs[(pointIdx * centroids->numGroups) + centroids->groupID[centrIdx]])
      {
        // pointLwrs[(pointIdx * centroids->numGroups) + centroids->groupID[centrIdx]] = currDistance;
      }
    }
  }
}


/*

*/
void groupCentroids(CentroidData_t *centroids, int p)
{
  // operation variables
  PointData_t tempCentrPoints;
  CentroidData_t tempGroupCentroids;

  // create points out of centroids
  makePoints(&tempCentrPoints, centroids->k, centroids->dim, 1);  // let p=1 for math, but groups are not needed
  for (int i = 0; i < centroids->k * centroids->dim; i++)
  {
    tempCentrPoints.coords[i] = centroids->coords[i];
  }

  // create p centroids for representing groups
  makeCentroids(&tempGroupCentroids, p, centroids->dim);
  startCentroids(&tempGroupCentroids, &tempCentrPoints);

  // run lloyd's on them with 'num-groups' as k
  run_seq_lloyd(&tempCentrPoints, &tempGroupCentroids, 10000);

  // assign groups based on results
  for (int i = 0; i < p; i++)
  {
    centroids->groupID[i] = tempCentrPoints.centroids[i];
  }

  // free temporary points and centroids
  freePoints(tempCentrPoints);
  freeCentroids(tempGroupCentroids);
}


/*

*/
void pointCalcsSimpleCPU(PointData_t *points, int pointIdx,
                        CentroidData_t *centroids, double *pointLwrPtr,
                        double *maxDriftArr, unsigned int *groupArr)
{
  // index variables
  unsigned int centIndex, grpIndex;
  double compDistance;

  for (grpIndex = 0; grpIndex < centroids->numGroups; grpIndex++)
  {
    // if the group is not blocked by group filter
    if (groupArr[pointIdx * centroids->numGroups + grpIndex])
    {
      // reset the lwrBoundArr to be only new lwrBounds
      pointLwrPtr[pointIdx * centroids->numGroups + grpIndex] = INFINITY;
    }
  }

  for (centIndex = 0; centIndex < centroids->k; centIndex++)
  {
    // if the centroid's group is marked in groupArr
    if (groupArr[pointIdx * centroids->numGroups + centroids->groupID[centIndex]])
    {
      // if it was the originally assigned cluster, no need to calc dist
      if (centIndex == points->prevCentroids[pointIdx])
      continue;

      // compute distance between point and centroid
      compDistance = calcSquaredEuclideanDist(points, pointIdx, centroids, centIndex);

      if (compDistance < points->ub[pointIdx])
      {
        pointLwrPtr[pointIdx * centroids->numGroups + centroids->groupID[points->centroids[pointIdx]]] = points->ub[pointIdx];
        points->centroids[pointIdx] = centIndex;
        points->ub[pointIdx] = compDistance;
      }
      else if(compDistance < pointLwrPtr[pointIdx * centroids->numGroups + centroids->groupID[centIndex]])
      {
        pointLwrPtr[pointIdx * centroids->numGroups + centroids->groupID[centIndex]] = compDistance;
      }
    }
  }
}


/*

*/
void run_seq_yin(PointData_t *pointList, CentroidData_t *centrList,
                  int numGroups, int maxIter)
{
  // operation variables
  unsigned int pntIndex, grpIndex;
  double tmpGlobLwr = INFINITY;
  // array to contain the maximum drift of each group of centroids
  // note: shared amongst all points
  double *maxDriftArr = (double *)malloc(sizeof(double) * numGroups);// array of all the points lower bounds
  double *pointLwrs = (double *)malloc(sizeof(double) * pointList->n * numGroups); // array of all the points lower bounds
  for(grpIndex = 0; grpIndex < pointList->n * numGroups; grpIndex++)
  {
    pointLwrs[grpIndex] = INFINITY; // initiatilize to INFINITY
  }
  // array to contain integer flags which mark which groups need to be checked
  // for a potential new centroid
  // note: unique to each point
  unsigned int *groupLclArr = (unsigned int *)malloc(sizeof(unsigned int) * pointList->n * numGroups);

  // select starting points for centroids and group centroids into p groups
  startCentroids(centrList, pointList);
  groupCentroids(centrList, numGroups);

  // run sequential lloyd's for first iteration and assign lower and upper bounds
  // run_seq_lloyd(pointList, centrList, 1);
  initPoints(pointList, centrList, pointLwrs);

  // while no convergence and not at max iterations
  // note, counter starting at 1 because at this point starting at 2nd iteration
  // for(int iterationCntr = 1; iterationCntr < maxIter; iterationCntr++)
  // {
  //   // clear drift array each new iteration
  //   for(grpIndex = 0; grpIndex < numGroups; grpIndex++)
  //   {
  //     maxDriftArr[grpIndex] = 0.0;
  //   }
  //
  //   // update centers via optimised update method
  //   updateCentroids_yinyang(centrList, pointList, maxDriftArr, numGroups);
  //
  //   for(pntIndex = 0; pntIndex < pointList->n; pntIndex++)
  //   {
  //     // reset old centroid before possibly finding a new one
  //     pointList->prevCentroids[pntIndex] = pointList->centroids[pntIndex];
  //
  //     tmpGlobLwr = INFINITY;
  //
  //     // update upper bound
  //         // ub = ub + centroid's drift
  //     pointList->ub[pntIndex] += centrList->drift[pointList->centroids[pntIndex]];
  //
  //     // update group lower bounds
  //         // lb = lb - maxGroupDrift
  //     for(grpIndex = 0; grpIndex < numGroups; grpIndex++)
  //     {
  //       pointLwrs[(pntIndex * numGroups) + grpIndex] -= maxDriftArr[grpIndex];
  //
  //       if(pointLwrs[(pntIndex * numGroups) + grpIndex] < tmpGlobLwr)
  //       {
  //           // minimum lower bound
  //           tmpGlobLwr = pointLwrs[(pntIndex * numGroups) + grpIndex];
  //       }
  //     }
  //
  //     // global filtering
  //     // if global lowerbound >= upper bound
  //     if (tmpGlobLwr < pointList->ub[pntIndex])
  //     {
  //       // tighten upperbound ub = d(x, b(x))
  //       pointList->ub[pntIndex] = calcSquaredEuclideanDist(pointList, pntIndex,
  //                                       centrList, pointList->centroids[pntIndex]);
  //
  //       // check condition again
  //       if (tmpGlobLwr < pointList->ub[pntIndex])
  //       {
  //         // group filtering
  //         for(grpIndex = 0; grpIndex < numGroups; grpIndex++)
  //         {
  //           // mark groups that need to be checked
  //           if (pointLwrs[(pntIndex * numGroups) + grpIndex] < pointList->ub[pntIndex])
  //           {
  //             groupLclArr[(pntIndex * numGroups) + grpIndex] = 1;
  //           }
  //           else
  //           {
  //             groupLclArr[(pntIndex * numGroups) + grpIndex] = 0;
  //           }
  //         }
  //
  //         // pass group array and point to go execute distance calculations
  //         pointCalcsSimpleCPU(pointList, pntIndex, centrList, pointLwrs, maxDriftArr, groupLclArr);
  //       }
  //     }
  //   }
  //
  //   // check for convergence
  //   if (checkConvergence(centrList))
  //   {
  //     // printf("Iterations: %d\n", iterationCntr+1);
  //     break;
  //   }
  // } /* end for  */
  //
  // // update centers via optimised update method
  // updateCentroids_yinyang(centrList, pointList, maxDriftArr, numGroups);

  free(maxDriftArr);
  free(pointLwrs);
  free(groupLclArr);
}
