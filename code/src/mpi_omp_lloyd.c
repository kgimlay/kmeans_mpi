// Author: Kevin Imlay

#include "../inc/mpi_lloyd.h"


/*

*/
void run_mpi_omp_lloyd(PointData_t *pointList, CentroidData_t *centrList, int maxIter,
                  int mpi_numProc, int mpi_rank)
{
  // double startTime, endTime;

  /** begin processes divergence on rank **/

  // select subpoint list
  int pointSublist_size_0_rank = calcPointSublistSize_rank0(pointList->n, mpi_numProc);
  int pointSublist_size_non_0_rank = calcPointSublistSize_rankNon0(pointList->n, mpi_numProc);
  if (mpi_rank != 0)
  {
    // select points for process
    pointList->sublistN = pointSublist_size_non_0_rank;
    pointList->sublistOffset = pointSublist_size_0_rank + ((mpi_rank - 1) * pointSublist_size_non_0_rank);
  }
  else
  {
    // select points for process
    pointList->sublistN = pointSublist_size_0_rank;
    pointList->sublistOffset = 0;
  }


    // printf("Rank %d sublist: %d, offset: %d\n", mpi_rank, pointList->sublistN, pointList->sublistOffset);

  /** end processes divergence on rank **/

  // select starting points for centroids
  startCentroids(centrList, pointList);

  // while no convergence and not at max iterations
  for(int iterationCntr = 0; iterationCntr < maxIter; iterationCntr++)
  {
    // prime centroids for next iteration
    // startTime = omp_get_wtime();
    primeCentroid(centrList);
    // endTime = omp_get_wtime();
    // printf("primeCentroid: %.3f\n", endTime-startTime);

    // re-member points to clusters
    // startTime = omp_get_wtime();
    assignCentroids_omp(pointList, centrList);
    // endTime = omp_get_wtime();
    // printf("assignCentroids_omp: %.3f\n", endTime-startTime);

    // recalculate center of clusters
    // pricess divergence on rank inside of here
    // startTime = omp_get_wtime();
    updateCentroids_MPI_OMP(pointList, centrList, mpi_rank, mpi_numProc,
                        pointSublist_size_0_rank, pointSublist_size_non_0_rank);
    // endTime = omp_get_wtime();
    // printf("updateCentroids_MPI_OMP: %.3f\n", endTime-startTime);

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

}
