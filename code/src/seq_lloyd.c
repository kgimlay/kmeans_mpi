// Author: Kevin Imlay

#include "../inc/seq_lloyd.h"


/*

*/
void run_seq_lloyd(Point *pointList, int pointList_size, Centroid *centrList,
                    int centrList_size, int maxIter)
{
  // select starting points for centroids
  startCentroids(centrList, centrList_size, pointList, pointList_size);

  // while no convergence and not at max iterations
  for(int iterationCntr = 0; iterationCntr < maxIter; iterationCntr++)
  {
    // prime centroids for next iteration
    primeCentroid(centrList, centrList_size);


    // re-member points to clusters
    updatePointClusterMembership(pointList, pointList_size,
                                  centrList, centrList_size);


    // recalculate center of clusters
    updateCentroids(pointList, pointList_size,
                    centrList, centrList_size);

    // check for convergence
    if (checkConvergence(centrList, centrList_size))
    {
      break;
    }

  } /* end for  */
}
