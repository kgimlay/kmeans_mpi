// Author: Kevin Imlay

#include "../inc/seq_lloyd.h"


/*

*/
void run_lin_lloyd(Point *pointList, int pointList_size, Centroid *centrList,
                    int centrList_size, int maxIter)
{
  // operation variables
  int iterationCntr;
  bool convergenceFlag = false;

  // select starting points for centroids
  startCentroids(centrList, centrList_size, pointList, pointList_size);

  // while no convergence and not at max iterations
  for(iterationCntr = 0; iterationCntr < maxIter && !convergenceFlag; iterationCntr++)
  {
    // update convergence to assume true
    // is changed if found to be false
    convergenceFlag = true;

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

  } /* end while */
}
