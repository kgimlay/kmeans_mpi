// Author: Kevin Imlay

#include "../inc/data_utils.h"


void makePoints(PointData_t *pointStruct, int n, int dim)
{
  // allocate lists
  pointStruct->centroid = (int*)malloc(sizeof(int) * n);
  pointStruct->coords   = (double*)malloc(sizeof(double) * n * dim);
  pointStruct->lb       = (double*)malloc(sizeof(int) * n);
  pointStruct->ub       = (double*)malloc(sizeof(int) * n);

  // assign values
  pointStruct->n        = n;
  pointStruct->dim      = dim;
}


void makeCentroids(CentroidData_t *centroidStruct, int k, int dim)
{
  // allocate lists
  centroidStruct->groupID    = (int*)malloc(sizeof(int) * k);
  centroidStruct->sizes      = (int*)malloc(sizeof(int) * k);
  centroidStruct->coords     = (double*)malloc(sizeof(double) * k * dim);
  centroidStruct->prevCoords = (double*)malloc(sizeof(double) * k * dim);
  centroidStruct->maxDrift   = (double*)malloc(sizeof(int) * k);

  // assign values
  centroidStruct->k          = k;
  centroidStruct->dim        = dim;
}


void freePoints(PointData_t pointList, int n)
{
  // free pointList fields
  free(pointList.centroid);
  free(pointList.coords);
  free(pointList.lb);
  free(pointList.ub);
}


void freeCentroids(CentroidData_t centroidList, int k)
{
  // free centroidList fields
  free(centroidList.groupID);
  free(centroidList.sizes);
  free(centroidList.coords);
  free(centroidList.prevCoords);
  free(centroidList.maxDrift);
}


/*

*/
double calcSquaredEuclideanDist(PointData_t *points, int pointId,
  CentroidData_t *centroids, int centroidId)
{
  // operaiton variables
  int dimensionality = points->dim;
  double tempSumSquared = 0.0;
  double tempPointDimCoord;
  double tempCentrDimCoord;

  // loop over each dimension
  for(int dimIdx = 0; dimIdx < dimensionality; dimIdx++)
  {
    // get dimensional coordinates
    tempPointDimCoord = points->coords[pointId * points->dim + dimIdx];
    tempCentrDimCoord = centroids->prevCoords[centroidId * centroids->dim + dimIdx]; // should be coords, not prevCoords?

    // difference, square, and sum
    tempSumSquared += pow(tempPointDimCoord - tempCentrDimCoord, 2);
  }

  return tempSumSquared;
}



/*

*/
void primeCentroid(CentroidData_t *centroidList)
{
  // loop over each centroid
  for(int centrIdx = 0; centrIdx < centroidList->k; centrIdx++)
  {
    // store current centroid pos into previous
    for(int dimIdx = 0; dimIdx < centroidList->dim; dimIdx++)
    {
      // centroidList[centrIdx].prevCoords[dimIdx] = centroidList[centrIdx].coords[dimIdx];
      // centroidList[centrIdx].coords[dimIdx] = 0.0;
      centroidList->prevCoords[centrIdx * centroidList->dim + dimIdx]
        = centroidList->coords[centrIdx * centroidList->dim + dimIdx];
      centroidList->coords[centrIdx * centroidList->dim + dimIdx] = 0.0;
    }

    // reset size for average calculation
    // centroidList[centrIdx].size = 0;
    centroidList->sizes[centrIdx] = 0;
  }
}


/*

*/
bool checkConvergence(CentroidData_t *centrList)
{
  for(int centrIdx = 0; centrIdx < centrList->k; centrIdx++)
  {
    for(int dimIdx = 0; dimIdx < centrList[centrIdx].dim; dimIdx++)
    {
      if(centrList[centrIdx].coords[dimIdx] != centrList[centrIdx].prevCoords[dimIdx]
          && !isnan(centrList[centrIdx].coords[dimIdx]))
      {
        return true;
      }
    } /* end for */
  }
  return false;
}


/*

*/
void updatePointClusterMembership(PointData_t *pointList,
                                  CentroidData_t *centroidList)
{
  // operation variables
  double tempMinDist;
  double tempDist;
  int tempCentr = -1;

  // loop over each point
  for(int pointIdx = 0; pointIdx < pointList->n; pointIdx++)
  {
    tempMinDist = INFINITY;

    // loop over each centroid for distance calculation
    for(int centrIdx = 0; centrIdx < centroidList->k; centrIdx++)
    {
      // calculate distance to each centroid
      tempDist = calcSquaredEuclideanDist(pointList, pointIdx, centroidList, centrIdx);

      // store current minimum
      if(tempDist < tempMinDist)
      {
        tempCentr = centrIdx;
        tempMinDist = tempDist;
      }
    }

    // update cluster membership
    // pointList[pointIdx].centroid = tempCentr;
    pointList->centroid[pointIdx] = tempCentr;
  } /* end for */
}


/*

*/
void updateCentroids(CentroidData_t *centrList, PointData_t *pointList)
{
  // loop over each point
  for(int pointIdx = 0; pointIdx < pointList->n; pointIdx++)
  {
    // for each dimension, sum into centroid's coords
    for(int dimIdx = 0; dimIdx < pointList->dim; dimIdx++)
    {
      centrList->coords[pointList->centroid[pointIdx] * centrList->dim + dimIdx]
        += pointList->coords[pointIdx * pointList->dim + dimIdx];
    }

    // update number of points in cluster
    (centrList->sizes[pointList->centroid[pointIdx]])++;
  } /* end for */


  // loop over each centroid and average coords
  for(int centrIdx = 0; centrIdx < centrList->k; centrIdx++)
  {
    // average
    for(int dimIdx = 0; dimIdx < centrList->dim; dimIdx++)
    {
      centrList->coords[centrIdx * centrList->dim + dimIdx]
        /= centrList->sizes[centrIdx];
    }
  } /* end for */
}


/*

*/
void startCentroids(CentroidData_t *centrList, PointData_t *pointList)
{
  // assign points to centroids, alternating
  // this helps with testing script; signifies disorder to begin
  for(int pointIdx = 0; pointIdx < pointList->n; pointIdx++)
  {
    pointList->centroid[pointIdx] = pointIdx % centrList->k;
  }

  /* select which method of setting starting positions for centroids */

  // first N datapoints
  #if CENTR_START_METHOD == 0
    // loop over centroids and pick datapoint n to set it's coords to
    for(int centrIdx = 0; centrIdx < centrList->k; centrIdx++)
    {
      // set coords to coords of point n
      for(int dimIdx = 0; dimIdx < centrList->dim; dimIdx++)
      {
        centrList->coords[centrIdx * centrList->dim + dimIdx]
          = pointList->coords[centrIdx * centrList->dim + dimIdx];
      }
    }
  #endif

  // spanning mean of alternating points
  #if CENTR_START_METHOD == 1
    // set centroid initial locations
    updateCentroids(centrList, pointList);
  #endif

  // random datapoints
  #if CENTR_START_METHOD == 2
    // random seed
    srand(RAND_SEED);

    // loop over centroids and pick a random datapoint to set it's coords to
    for(int centrIdx = 0; centrIdx < centrList->k; centrIdx++)
    {
      // set coords to coords of a random point
      for(int dimIdx = 0; dimIdx < centrList->dim; dimIdx++)
      {
        centrList->coords[centrIdx * centrList->dim + dimIdx]
          = pointList->coords[(rand() % pointList->n) * pointList->dim + dimIdx];
          printf("%.3f, ", pointList->coords[(rand() % pointList->n) * pointList->dim + dimIdx]);
      }
      printf("\n");
    }
  #endif
}


/*

*/
void updateCentroids_MPI(PointData_t *pointSublist, CentroidData_t *centrList,
                      int mpi_rank, int mpi_numProc, double *mpiCentrDataList,
                      int mpiCentrDataList_width)
{
  int dataDim = pointSublist->dim;

  // calculate weighted means
  for (int i = 0; i < pointSublist->n; i++)
  {
    for (int j = 0 ; j < pointSublist->dim; j++)
    {
      mpiCentrDataList[pointSublist->centroid[i] * mpiCentrDataList_width + j]
        += pointSublist->coords[i * pointSublist->dim + j];
    }
    mpiCentrDataList[pointSublist->centroid[i] * mpiCentrDataList_width + centrList->dim]++;
  }


  /** begin processes divergence on rank **/

  // if not rank 0, send weighted mean to rank 0, then wait for new centroid
  // locations
  if (mpi_rank != 0)
  {
    // printf("Rank %d sending to Rank 0\n", mpi_rank);
    // send to rank 0
    MPI_Send(mpiCentrDataList,
      centrList->k * (centrList->dim + 1),
      MPI_DOUBLE,
      0, 0,
      MPI_COMM_WORLD);
    // printf("Rank %d, done sending\n", mpi_rank);
    // receive new centroid locations from rank 0
    MPI_Status status;
    MPI_Recv(mpiCentrDataList,
      centrList->k * mpiCentrDataList_width,
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
    for (int i = 1; i < mpi_numProc; i++)
    {
      // printf("Rank 0 receiving from Rank %d\n", i);
      MPI_Recv(&mpiCentrDataList[i * mpiCentrDataList_width * centrList->k],
        centrList->k * mpiCentrDataList_width,
        MPI_DOUBLE,
        i, 0,
        MPI_COMM_WORLD,
        &status);
      // printf("Rank 0, done receiving from rank %d\n", i);
    }

    // recalculate center of clusters
    for (int i = 0; i < centrList->k; i++)
    {
      for (int j = 0; j < mpiCentrDataList_width; j++)
      {
        for (int k = 1; k < mpi_numProc; k++)
        {
          mpiCentrDataList[i * mpiCentrDataList_width + j] +=
            mpiCentrDataList[i * mpiCentrDataList_width + j + k * mpiCentrDataList_width * centrList->k];
        }
      }
    }
    for (int i = 0; i < centrList->k; i++)
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
        centrList->k * mpiCentrDataList_width,
        MPI_DOUBLE,
        i, 0,
        MPI_COMM_WORLD);
    }
  }

  /** end processes divergence on rank **/

  for (int i = 0; i < centrList->k; i++)
  {
    for (int j = 0; j < dataDim; j++)
    {
      centrList->coords[i * centrList->dim + j] = mpiCentrDataList[i * mpiCentrDataList_width + j];
    }
  }
}
