// Author: Kevin Imlay

#include "../inc/mpi_yinyang.h"


/*

*/
static void groupCentroids(CentroidData_t *centroids, int t)
{
  // operation variables
  int numCentroids = centroids->k;
  int dim = centroids->dim;
  PointData_t centPoints;
  CentroidData_t centCentroids;

  // printf("::: %d * %d = %d\n", numCentroids, t, numCentroids*t);

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
static void run_yinyang_firstItr(PointData_t *pointList, CentroidData_t *centroidList,
                          int numGroups)
{
    // operation variables
    double tempDist;

    // loop over each point
    for (int pointIdx = 0; pointIdx < pointList->n; pointIdx++)
    {
      pointList->ub[pointIdx] = INFINITY;

      // loop over each centroid for distance calculation
      for (int centrIdx = 0; centrIdx < centroidList->k; centrIdx++)
      {
        // calculate distance to each centroid
        tempDist = calcSquaredEuclideanDist_yinyang(pointList, pointIdx, centroidList, centrIdx);

        // store current minimum
        if (tempDist < pointList->ub[pointIdx])
        {
          if (pointList->ub[pointIdx] != INFINITY)
          {
            pointList->lb[pointIdx * numGroups + centroidList->groupID[pointList->centroids[pointIdx]]] = pointList->ub[pointIdx];
          }
          pointList->centroids[pointIdx] = centrIdx;
          pointList->ub[pointIdx] = tempDist;
        }
        else if (tempDist < pointList->lb[pointIdx * numGroups + centroidList->groupID[centrIdx]])
        {
          pointList->lb[pointIdx * numGroups + centroidList->groupID[centrIdx]] = tempDist;
        }
      } /* end for */

    } /* end for */
}


/*

*/
void run_mpi_yin(PointData_t *pointList, CentroidData_t *centrList,
                  int numGroups, int maxIter, int mpi_numProc, int mpi_rank)
{
  // operation variables
  int tempDist;
  double *maxDriftArr = (double *)malloc(sizeof(double) * numGroups);
  double tmpGlobLwr = INFINITY;
  bool *groupLclArr = (bool *)malloc(sizeof(bool) * pointList->n * numGroups);

  if (maxDriftArr == NULL || groupLclArr == NULL)
  {
    printf("Problem allocating memory! [mpi_yinyang.c]\n");
  }

  // initiatilize to INFINITY
  for(int i = 0; i < pointList->n * numGroups; i++)
  {
    pointList->lb[i] = INFINITY;
  }

  /** begin processes divergence on rank **/

  // select subpoint list
  int pointSublist_size_0_rank = calcPointSublistSize_rank0(pointList->n, mpi_numProc);
  int pointSublist_size_non_0_rank = calcPointSublistSize_rankNon0(pointList->n, mpi_numProc);
  if (mpi_rank != 0)
  {
    // select points for process
    pointList->sublistN = pointSublist_size_non_0_rank;
    pointList->sublistOffset = pointSublist_size_0_rank + ((mpi_rank - 1) * pointSublist_size_non_0_rank);
  }
  else
  {
    // select points for process
    pointList->sublistN = pointSublist_size_0_rank;
    pointList->sublistOffset = 0;
  }

  // printf("Rank %d sublist: %d, offset: %d\n", mpi_rank, pointList->sublistN, pointList->sublistOffset);

  /** end processes divergence on rank **/


    // printf("::: %d * %d = %d\n", centrList->k, numGroups, centrList->k*numGroups);

  // cluster the centroids into t groups
  startCentroids(centrList, pointList);
  groupCentroids(centrList, numGroups);

  // run 1 iteration of Lloyd's for initial cluster assignments
  run_yinyang_firstItr(pointList, centrList, numGroups);
  // run_seq_lloyd(pointList, centrList, 1);

  // printf("Centroids:\n");
  // for (int i = 0; i < centrList->k; i++)
  // {
  //   for (int j = 0; j < centrList->dim; j++)
  //   {
  //     printf("%.2f, ", centrList->coords[i * centrList->dim + j]);
  //   }
  //   printf("\n");
  // }
  // printf("\n");

  // now run Yinyang iterations
  for (int iterationCntr = 1; iterationCntr < maxIter; iterationCntr++)
  {
    // printf("Iteration: %d\n", iterationCntr);

    // prime for next iteration
    primeCentroid(centrList);
    for(int groupIdx = 0; groupIdx < numGroups; groupIdx++)
    {
      maxDriftArr[groupIdx] = 0.0;
    }

    // update step
    // updateCentroids_yinyang(centrList, pointList, maxDriftArr, numGroups);
    updateCentroids_yinyangMPI(centrList, pointList, maxDriftArr, numGroups,
                              mpi_rank, mpi_numProc, pointSublist_size_0_rank,
                              pointSublist_size_non_0_rank);

    // for(int groupIdx = 0; groupIdx < numGroups; groupIdx++)
    // {
    //   printf("Max Drift: %.3f\n", maxDriftArr[groupIdx]);
    // }


    // filtering
    // done in parallel
    for(int pointIdx = pointList->sublistOffset;
      pointIdx < pointList->sublistN + pointList->sublistOffset;
      pointIdx++)
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
        pointList->ub[pointIdx] = calcSquaredEuclideanDist_yinyang(pointList, pointIdx, centrList, pointList->centroids[pointIdx]);

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
            if (groupLclArr[pointIdx * numGroups + centrList->groupID[centroidIdx]])
            {
              // skip if centroid assignment did not change
              if (centroidIdx == pointList->prevCentroids[pointIdx])
              {
                continue;
              }

              // compute distance between point and centroid
              tempDist = calcSquaredEuclideanDist_yinyang(pointList, pointIdx, centrList, centroidIdx);

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

    // printf("Centroids:\n");
    // for (int i = 0; i < centrList->k; i++)
    // {
    //   for (int j = 0; j < centrList->dim; j++)
    //   {
    //     printf("%.2f, ", centrList->coords[i * centrList->dim + j]);
    //   }
    //   printf("\n");
    // }
    // printf("\n");





    // check for convergence
    if (checkConvergence(centrList))
    {
      printf("Iterations: %d\n", iterationCntr+1);
      break;
    }
  }

  // recalculate center of clusters
  // updateCentroids_yinyang(centrList, pointList, maxDriftArr, numGroups);
  updateCentroids_yinyangMPI(centrList, pointList, maxDriftArr, numGroups,
                            mpi_rank, mpi_numProc, pointSublist_size_0_rank,
                            pointSublist_size_non_0_rank);
}
