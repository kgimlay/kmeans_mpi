// Author: Kevin Imlay

#include "../inc/lin_lloyd.h"


/*

*/
void run_mpi_lloyd(Point *pointList, int pointList_size, Centroid *centrList,
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
    for(int centrIdx = 0; centrIdx < centrList_size; centrIdx++)
    {
      for(int dimIdx = 0; dimIdx < centrList[centrIdx].dim; dimIdx++)
      {
        if(centrList[centrIdx].coords[dimIdx] != centrList[centrIdx].prevCoords[dimIdx]
            && !isnan(centrList[centrIdx].coords[dimIdx]))
        {
          convergenceFlag = false;
        }
      } /* end for */
    }

  } /* end while */

  // if convergence not reached in max iterations
  if (!convergenceFlag)
  {
    printf("Convergence not reached in %d iterations\n", iterationCntr);
  }
  else
  {
    // convergence reached
    printf("Convergence reached in %d iterations\n", iterationCntr);
  }
}
