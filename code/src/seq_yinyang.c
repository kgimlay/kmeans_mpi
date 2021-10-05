// Author: Keivn Imlay

#include "../inc/seq_yinyang.h"

void run_lin_yin(Point *pointList, int pointList_size, Centroid *centrList,
                    int centrList_size, int numGroups, int maxIter)
{
  // local groups
  // Centroid *groups = (Centroid *)malloc(sizeof(Centroid *) * numGroups);
  // makeCentroids(groups, numGroups, centrList[0].dim);

  // select starting points for centroids
  startCentroids(centrList, centrList_size, pointList, pointList_size);

  // group centroids into t groups
  {
    Point *centrPoints = (Point *)malloc(sizeof(Point) * centrList_size);
    makePoints(centrPoints, centrList_size, centrList[0].dim);
    centrToPoint(centrList, centrList_size, centrPoints);
    
    freePoints(centrPoints, centrList_size);
  }

  // run sequential lloyd's for first iteration and assign lower and upper bounds
  run_lin_lloyd(pointList, pointList_size, centrList, centrList_size, 1);

  // while no convergence and not at max iterations
  // note, counter starting at 1 because at this point starting at 2nd iteration
  for(int iterationCntr = 1; iterationCntr < maxIter; iterationCntr++)
  {



    // check for convergence
    if (checkConvergence(centrList, centrList_size))
    {
      break;
    }

  } /* end for  */
}
