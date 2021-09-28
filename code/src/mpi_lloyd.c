// Author: Kevin Imlay

#include "../inc/mpi_lloyd.h"


/*

*/
void run_mpi_lloyd(Point *pointList, int pointList_size, Centroid *centrList,
                    int centrList_size, int maxIter, int mpi_numProc, int mpi_rank)
{
  // operation variables
  Point *pointSublist;
  int pointSublist_size;
  int dataDim = pointList[0].dim;
  double *mpiCentrDataList;
  int mpiCentrDataList_length;
  int mpiCentrDataList_width = dataDim + 1;

  // allocate weighted mean location list for MPI communication
  if (mpi_rank != 0)
  {
    mpiCentrDataList_length = centrList_size;
  }
  else
  {
    mpiCentrDataList_length = centrList_size * mpi_numProc;
  }
  mpiCentrDataList = (double *)malloc(sizeof(double) * mpiCentrDataList_length * mpiCentrDataList_width);

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
  // printf("Rank %d taking points %d thru %d\n", mpi_rank, pointSublist[0].id, pointSublist[pointSublist_size-1].id);

  /** end processes divergence on rank **/

  // while no convergence and not at max iterations
  for(int iterationCntr = 0; iterationCntr < maxIter; iterationCntr++)
  {
    // prime centroids for next iteration
    primeCentroid(centrList, centrList_size);

    // prime centroid weighted average list for MPI
    for (int i = 0; i < mpiCentrDataList_length * mpiCentrDataList_width; i++)
    {
      mpiCentrDataList[i] = 0;
    }

    // re-member points to clusters
    updatePointClusterMembership(pointSublist, pointSublist_size,
                                  centrList, centrList_size);

    // recalculate center of clusters
    updateCentroids_MPI(pointSublist, pointSublist_size, centrList,
                        centrList_size, mpi_rank, mpi_numProc, mpiCentrDataList,
                        mpiCentrDataList_width);

    // check for convergence
    if (checkConvergence(centrList, centrList_size))
    {
      break;
    }

  } /* end while */

  // communicate to set rank 0's point data (centroid assignment) to reflect
  // the distributed conclusion


  // free memory
  free(mpiCentrDataList);
}
