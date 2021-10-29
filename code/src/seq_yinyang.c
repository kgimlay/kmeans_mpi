// Author: Keivn Imlay

#include "../inc/seq_yinyang.h"



/*

*/
void groupCentroids(CentroidData_t *centroids, int num_groups, CentroidData_t *groups)
{
  // create points out of centroids


  // run lloyd's on them with 'num-groups' as k


  // assign groups based on results

}


/*

*/
void pointCalcsSimpleCPU(PointData_t *points, int pointIdx,
                        CentroidData_t *centroids, CentroidGroupData_t *groups)
{
  // index variables
  unsigned int centIndex, grpIndex;

  double compDistance;

  for (grpIndex = 0; grpIndex < groups->numGroups; grpIndex++)
  {
    // if the group is not blocked by group filter
    if (groups->groupLcl[grpIndex])
    {
      // reset the lwrBoundArr to be only new lwrBounds
      groups->groupLcl[grpIndex] = INFINITY;
    }
  }

  for (centIndex = 0; centIndex < centroids->k; centIndex++)
  {
    // if the centroid's group is marked in groupArr
    if (groups->centroidAss[centIndex])
    {
      // if it was the originally assigned cluster, no need to calc dist
      if (centIndex == points->prevCentroids[pointIdx])
      {
        continue;
      }

      // compute distance between point and centroid
      compDistance = calcSquaredEuclideanDist(points, pointIdx, centroids, points->centroids[pointIdx]);
      if(compDistance < points->ub[pointIdx])
      {
        points->lb[groups->centroidAss[pointIdx]] = points->ub[pointIdx];
        points->centroids[pointIdx] = centIndex;
        points->ub[pointIdx] = compDistance;
      }
      else if(compDistance < points->lb[groups->centroidAss[centIndex]])
      {
        points->lb[groups->centroidAss[pointIdx]] = compDistance;
      }
    }
  }
}


/*

*/
void run_lin_yin(PointData_t *pointList, CentroidData_t *centrList,
                  int numGroups, int maxIter)
{
  // operation variables
  double tmpGlobLwr = INFINITY;

  // create centroid group data
  CentroidGroupData_t centroidGroups;
  makeCentroidGroups(&centroidGroups, numGroups, pointList->n);

  // select starting points for centroids and group centroids into p groups
  startCentroids(centrList, pointList);
  groupCentroids(centrList, numGroups, &centroidGroups); // TODO: implement

  // run sequential lloyd's for first iteration and assign lower and upper bounds
  run_seq_lloyd(pointList, centrList, 1);
  // TODO: assign bounds

  // while no convergence and not at max iterations
  // note, counter starting at 1 because at this point starting at 2nd iteration
  for(int iterationCntr = 1; iterationCntr < maxIter; iterationCntr++)
  {
    // clear drift array each new iteration
    for(int i = 0; i < numGroups; i++)
    {
      centroidGroups.maxDrift[1] = 0.0;
    }

    // update centers via optimised update method
    updateCentroids_yinyang(centrList, pointList, &centroidGroups);   // TODO: implement

    // loop over all points for filtering
    for (int pointIdx = 0; pointIdx < pointList->n; pointIdx++)
    {
      // reset old centroid before possibly finding a new one
      pointList->prevCentroids[pointIdx] = pointList->centroids[pointIdx];
      tmpGlobLwr = INFINITY;

      // update upper bound (ub = ub + centroid's drift)
      pointList->ub[pointIdx] += centroidGroups.maxDrift[pointList->centroids[pointIdx]];

      // update group lower bounds (lb = lb - maxGroupDrift)
      for(int grpIndex = 0; grpIndex < numGroups; grpIndex++)
      {
        // pointLwrs[(pntIndex * numGrp) + grpIndex] -= maxDriftArr[grpIndex];
        pointList->lb[pointIdx * centroidGroups.numGroups + grpIndex]
            -= centroidGroups.maxDrift[grpIndex];

        // if(pointLwrs[(pntIndex * numGrp) + grpIndex] < tmpGlobLwr)
        if (pointList->lb[pointIdx * centroidGroups.numGroups + grpIndex] < tmpGlobLwr)
        {
            // minimum lower bound
            tmpGlobLwr = pointList->lb[pointIdx * centroidGroups.numGroups + grpIndex];
        }
      }

      // global filtering
      // if global lowerbound >= upper bound
      if(tmpGlobLwr < pointList->ub[pointIdx])
      {
        // tighten upperbound ub = d(x, b(x))
        pointList->ub[pointIdx] = calcSquaredEuclideanDist(pointList, pointIdx, centrList, pointList->centroids[pointIdx]);

        // check condition again
        if(tmpGlobLwr < pointList->ub[pointIdx])
        {
          // group filtering
          for(int grpIndex = 0; grpIndex < numGroups; grpIndex++)
          {
            // mark groups that need to be checked
            if (pointList->lb[pointIdx * numGroups + grpIndex] < pointList->ub[pointIdx])
            {
              centroidGroups.groupLcl[pointIdx * numGroups + grpIndex] = 1;
            }
            else
            {
              centroidGroups.groupLcl[pointIdx * numGroups + grpIndex] = 1;
            }
          }

          // IDK what this does
          pointCalcsSimpleCPU(pointList, pointIdx,centrList, &centroidGroups);
        }
      }
    }

    // check for convergence
    if (checkConvergence(centrList))
    {
      break;
    }

  } /* end for  */

  // update centers via optimised update method
  updateCentroids_yinyang(centrList, pointList, &centroidGroups);   // TODO: implement

  // free memory
  freeCentroidGroups(centroidGroups);
}
