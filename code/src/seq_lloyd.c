// Author: Kevin Imlay

#include "../inc/seq_lloyd.h"


/*

*/
void run_seq_lloyd(PointData_t *pointList, CentroidData_t *centrList, int maxIter)
{
  // select starting points for centroids
  startCentroids(centrList, pointList);

  // while no convergence and not at max iterations
  for(int iterationCntr = 0; iterationCntr < maxIter; iterationCntr++)
  {
    // prime centroids for next iteration
    primeCentroid(centrList);


    // re-member points to clusters
    updatePointClusterMembership(pointList, centrList);


    // recalculate center of clusters
    updateCentroids(centrList, pointList);

    // check for convergence
    if (checkConvergence(centrList))
    {
      break;
    }

  } /* end for  */
}
