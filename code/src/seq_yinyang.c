// Author: Keivn Imlay

#include "../inc/seq_yinyang.h"


/*

*/
void groupCentroids(CentroidData_t *centroids, int num_groups)
{

}


/*

*/
void run_lin_yin(PointData_t *pointList, CentroidData_t *centrList,
                  int numGroups, int maxIter)
{
  // select starting points for centroids
  startCentroids(centrList, pointList);

  // group centroids into t groups
  groupCentroids(centrList, numGroups);

  // run sequential lloyd's for first iteration and assign lower and upper bounds
  run_seq_lloyd(pointList, centrList, 1);

  // while no convergence and not at max iterations
  // note, counter starting at 1 because at this point starting at 2nd iteration
  for(int iterationCntr = 1; iterationCntr < maxIter; iterationCntr++)
  {
    // clear drift array each new iteration

    // update centers via optimised update method

    // filtering done in parallel

      // reset old centroid before possibly finding a new one

      // update upper bound (ub = ub + centroid's drift)

      // update group lower bounds (lb = lb - maxGroupDrift)

      // global filtering
      // if global lowerbound >= upper bound

        // tighten upperbound ub = d(x, b(x))

        // check condition again

          // group filtering

            // mark groups that need to be checked

          // pass group array and point to go execute distance calculations


    // check for convergence
    if (checkConvergence(centrList))
    {
      break;
    }

    // update centers via optimised update method

  } /* end for  */
}
