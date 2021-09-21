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

    // prime centroid weighted average list for MPI
    for (int i = 0; i < mpiCentrDataList_length * mpiCentrDataList_width; i++)
    {
      mpiCentrDataList[i] = 0;
    }

    // re-member points to clusters
    updatePointClusterMembership(pointSublist, pointSublist_size,
                                  centrList, centrList_size);

    // calculate weighted means
    for (int i = 0; i < pointSublist_size; i++)
    {
      Point *tempPoint = &pointSublist[i];
      for (int j = 0 ; j < tempPoint->dim; j++)
      {
        mpiCentrDataList[tempPoint->centroid->id * mpiCentrDataList_width + j] += tempPoint->coords[j];
      }
      mpiCentrDataList[tempPoint->centroid->id * mpiCentrDataList_width + centrList[0].dim]++;
    }


    /** begin processes divergence on rank **/

    // if not rank 0, send weighted mean to rank 0, then wait for new centroid
    // locations
    if (mpi_rank != 0)
    {
      // send to rank 0
      MPI_Send(mpiCentrDataList,
        centrList_size * (centrList[0].dim + 1),
        MPI_DOUBLE,
        0, 0,
        MPI_COMM_WORLD);

      // receive new centroid locations from rank 0
      MPI_Status status;
      MPI_Recv(mpiCentrDataList,
        centrList_size * mpiCentrDataList_width,
        MPI_DOUBLE,
        0, 0,
        MPI_COMM_WORLD,
        &status);
    }

    // if rank 0, recieve weighted sums, calculate new centroid locations,
    // and send locations back to all other ranks
    else
    {
      // receive weighted means
      MPI_Status status;
      for (int i = 1; i < mpi_numProc; i += centrList_size)
      {
        MPI_Recv(&mpiCentrDataList[i * mpiCentrDataList_width * centrList_size],
          centrList_size * mpiCentrDataList_width,
          MPI_DOUBLE,
          1, 0,
          MPI_COMM_WORLD,
          &status);
      }

      // recalculate center of clusters
      for (int i = 0; i < centrList_size; i++)
      {
        for (int j = 0; j < mpiCentrDataList_width; j++)
        {
          for (int k = 1; k < mpi_numProc; k++)
          {
            mpiCentrDataList[i * mpiCentrDataList_width + j] +=
              mpiCentrDataList[i * mpiCentrDataList_width + j + k * mpiCentrDataList_width * centrList_size];
          }
        }
      }
      for (int i = 0; i < centrList_size; i++)
      {
        for (int j = 0; j < mpiCentrDataList_width - 1; j++)
        {
          mpiCentrDataList[i * mpiCentrDataList_width + j] /=
            mpiCentrDataList[i * mpiCentrDataList_width + mpiCentrDataList_width - 1];
        }
      }

      for (int i = 1; i < mpi_numProc; i++)
      {
        MPI_Send(mpiCentrDataList,
          centrList_size * mpiCentrDataList_width,
          MPI_DOUBLE,
          i, 0,
          MPI_COMM_WORLD);
      }
    }

    /** end processes divergence on rank **/

    for (int i = 0; i < centrList_size; i++)
    {
      for (int j = 0; j < dataDim; j++)
      {
        centrList[i].coords[j] = mpiCentrDataList[i * mpiCentrDataList_width + j];
      }
    }

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

  free(mpiCentrDataList);
}
