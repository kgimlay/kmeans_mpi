// Author: Kevin Imlay

#include "../inc/mpi_lloyd.h"


/*

*/
void run_mpi_lloyd(PointData_t *pointList, CentroidData_t *centrList, int maxIter,
                  int mpi_numProc, int mpi_rank)
{
  // operation variables
  PointData_t pointSublist;
  int dataDim = pointList->dim;
  double *mpiCentrDataList;
  int mpiCentrDataList_length;
  int mpiCentrDataList_width = dataDim + 1;

  // allocate weighted mean location list for MPI communication
  if (mpi_rank != 0)
  {
    mpiCentrDataList_length = pointList->n/centrList->k;
  }
  else
  {
    mpiCentrDataList_length = pointList->n/centrList->k * mpi_numProc; // I think bug of dropping a couple points in computation stems from here
  }
  mpiCentrDataList = (double *)malloc(sizeof(double) * mpiCentrDataList_length * mpiCentrDataList_width);

  // select starting points for centroids
  startCentroids(centrList, pointList);

  /** begin processes divergence on rank **/

  // select points for process
  int pointSublist_size_non_0 = pointList->n / mpi_numProc;
  int pointSublist_size_0 = pointSublist_size_non_0 + pointList->n % mpi_numProc;
  if (mpi_rank == 0)
  {
    // make point sublist
    makePoints(&pointSublist, pointSublist_size_0, pointList->dim);

    // copy data to point sublist
    for (int i = 0; i < pointSublist_size_0 * pointList->dim; i++)
    {
      pointSublist.coords[i] = pointList->coords[i];
    }
  }
  else
  {
    // make point sublist
    makePoints(&pointSublist, pointSublist_size_non_0, pointList->dim);

    // copy data to point sublist
    for (int i = 0; i < pointSublist_size_non_0 * pointList->dim; i++)
    {
      pointSublist.coords[i] = pointList->coords[i + pointSublist_size_0 + (mpi_rank - 1) * pointSublist_size_non_0];
    }

    // offset the other ranks' sublist by the amount added to rank 0's size
    // pointSublist = pointList[mpi_rank * pointSublist.n + pointList->n % mpi_numProc];
  }
  // printf("Rank %d taking points %d thru %d\n", mpi_rank, pointSublist[0].id, pointSublist[pointSublist_size-1].id);

  /** end processes divergence on rank **/

  // while no convergence and not at max iterations
  for(int iterationCntr = 0; iterationCntr < maxIter; iterationCntr++)
  {
    // prime centroids for next iteration
    primeCentroid(centrList);

    // double tick = MPI_Wtime();

    // printf("Rank %d, list length: %d\n", mpi_rank, mpiCentrDataList_length);

    // prime centroid weighted average list for MPI
    for (int i = 0; i < mpiCentrDataList_length * mpiCentrDataList_width; i++)
    {
      mpiCentrDataList[i] = 0.0;
    }

    // double tock = MPI_Wtime();
    // if (mpi_rank == 0)
    // {
    // printf("MPI time: %.4f\n", tock-tick);
    // }

    // re-member points to clusters
    updatePointClusterMembership(&pointSublist, centrList);

    // recalculate center of clusters
    updateCentroids_MPI(&pointSublist, centrList, mpi_rank, mpi_numProc,
                        mpiCentrDataList, mpiCentrDataList_width);

    // check for convergence
    if (checkConvergence(centrList))
    {
      if (mpi_rank == 0)
      {
        printf("Iterations: %d\n", iterationCntr+1);
      }
      break;
    }

  }

  // communicate to set rank 0's point data (centroid assignment) to reflect
  // the distributed conclusion


  // free memory
  free(mpiCentrDataList);
}
