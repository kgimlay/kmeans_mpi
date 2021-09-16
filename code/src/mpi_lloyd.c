// Author: Kevin Imlay

#include "../inc/mpi_lloyd.h"


/*

*/
void run_mpi_lloyd(Point *pointList, int pointList_size, Centroid *centrList,
                    int centrList_size, int maxIter, int mpi_numProc, int mpi_rank)
{
  // operation variables
  int iterationCntr;
  bool convergenceFlag = false;
  Point *pointSublist;
  int pointSublistSize;

  // select starting points for centroids
  startCentroids(centrList, centrList_size, pointList, pointList_size);

  /** begin processes divergence **/
  // select points for process
  pointSublistSize = pointList_size / mpi_numProc;
  if (mpi_rank == 0)
  {
    // rank 0 takes the odd point from if the size of the point list is odd
    pointSublistSize += pointList_size % mpi_numProc;
    pointSublist = &pointList[mpi_rank * pointSublistSize];
  }
  else
  {
    // offset the other ranks' sublist by the amount added to rank 0's size
    pointSublist = &pointList[mpi_rank * pointSublistSize + pointList_size % mpi_numProc];
  }
  printf("Rank %d taking points %d thru %d\n", mpi_rank, pointSublist[0].id, pointSublist[pointSublistSize-1].id);

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

  /** end processes divergence **/
}
