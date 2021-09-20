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
  int pointSublist_size;

  // select starting points for centroids
  startCentroids(centrList, centrList_size, pointList, pointList_size);

  /** begin processes divergence on rank **/

  // select points for process
  pointSublist_size = pointList_size / mpi_numProc;
  if (mpi_rank == 0)
  {
    // rank 0 takes the odd point from if the size of the point list is odd
    pointSublist_size += pointList_size % mpi_numProc;
    pointSublist = &pointList[mpi_rank * pointSublist_size];
  }
  else
  {
    // offset the other ranks' sublist by the amount added to rank 0's size
    pointSublist = &pointList[mpi_rank * pointSublist_size + pointList_size % mpi_numProc];
  }
  printf("Rank %d taking points %d thru %d\n", mpi_rank, pointSublist[0].id, pointSublist[pointSublist_size-1].id);

  /** end processes divergence on rank **/

  // while no convergence and not at max iterations
  for(iterationCntr = 0; iterationCntr < maxIter && !convergenceFlag; iterationCntr++)
  {
    // update convergence to assume true
    // is changed if found to be false
    convergenceFlag = true;

    // prime centroids for next iteration
    primeCentroid(centrList, centrList_size);


    // re-member points to clusters
    updatePointClusterMembership(pointSublist, pointSublist_size,
                                  centrList, centrList_size);

    /** begin processes divergence on rank **/

    // if not rank 0, send weighted mean to rank 0, then wait for new centroid
    // locations
    if (mpi_rank != 0)
    {

      // calculate weighted means of all clusters
      // not actually caluclating mean, skipping the divide step to let
      // rank 0 do it
      // loop over each point

      // send to rank 0


      // receive new centroid locations from rank 0

    }

    // if rank 0, recieve weighted sums, calculate new centroid locations,
    // and send locations back to all other ranks
    else
    {
      // receive weighted means


      // recalculate center of clusters
      updateCentroids(pointSublist, pointSublist_size,
                      centrList, centrList_size);

      // send new locations

    }

    /** end processes divergence on rank **/

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
}
