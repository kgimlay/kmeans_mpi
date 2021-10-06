// Author: Keivn Imlay

#include "../inc/seq_yinyang.h"


/*

*/
void groupCentroids(Centroid *centroids, int size, int num_groups)
{
  // make points at centroid locations for running lloyd's to groupd centroids
  Point *centrPoints = (Point *)malloc(sizeof(Point) * size);
  makePoints(centrPoints, size, centroids[0].dim);
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < centrPoints[0].dim; j++)
    {
      centrPoints[i].coords[j] = centroids[i].coords[j];
    }
  }

  // make centroids for grouping centroids
  Centroid *centrCentroids = (Centroid *)malloc(sizeof(Centroid) * num_groups);
  makeCentroids(centrCentroids, num_groups, centroids[0].dim);

  // run lloyd's to groupd centroids (points at centroid locations)
  run_lin_lloyd(centrPoints, size, centrCentroids, num_groups, 10);

  // extract grouping from points to centroids
  for (int i = 0; i < size; i++)
  {
    centroids[i].groupId = centrPoints[i].centroid->id;
    // printf("Centroid %d grouped to %d\n", i, centrList[i].groupId);
  }

  // free points, not needed after grouping of centroids
  freePoints(centrPoints, size);
  freeCentroids(centrCentroids, num_groups);
}


/*

*/
void run_lin_yin(Point *pointList, int pointList_size, Centroid *centrList,
                    int centrList_size, int numGroups, int maxIter)
{
  // local groups
  // Centroid *groups = (Centroid *)malloc(sizeof(Centroid *) * numGroups);
  // makeCentroids(groups, numGroups, centrList[0].dim);

  // select starting points for centroids
  startCentroids(centrList, centrList_size, pointList, pointList_size);

  // group centroids into t groups
  groupCentroids(centrList, centrList_size, numGroups);

  // run sequential lloyd's for first iteration and assign lower and upper bounds
  run_lin_lloyd(pointList, pointList_size, centrList, centrList_size, 1);

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
    if (checkConvergence(centrList, centrList_size))
    {
      break;
    }

    // update centers via optimised update method

  } /* end for  */
}
