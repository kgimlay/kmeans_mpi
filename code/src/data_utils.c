// Author: Kevin Imlay

#include "../inc/data_utils.h"


/*

*/
void makePoints(PointData_t *pointStruct, int n, int dim, int p)
{
  // allocate lists
  pointStruct->centroids      = (int*)malloc(sizeof(int) * n);
  pointStruct->prevCentroids  = (int*)malloc(sizeof(int) * n);
  pointStruct->coords         = (double*)malloc(sizeof(double) * n * dim);
  pointStruct->ub             = (double*)malloc(sizeof(double) * n);
  pointStruct->lb             = (double*)malloc(sizeof(double) * n * p);

  // check errors with mem allocation
  if (pointStruct->centroids == NULL
      || pointStruct->prevCentroids == NULL
      || pointStruct->coords == NULL
      || pointStruct->lb == NULL
      || pointStruct->ub == NULL)
  {
    printf("Problem allocating memory [data_utils.c/makePoints]\n");
  }

  // assign values
  pointStruct->n              = n;
  pointStruct->dim            = dim;
  pointStruct->sublistN       = n;
  pointStruct->sublistOffset  = 0;

  // set lower bounds
  for (int i = 0; i < n * p; i++)
  {
    pointStruct->ub[i] = INFINITY;
  }

  /* For Debugging */
  // set centroid to all -1
  for (int i = 0; i < n; i++)
  {
    pointStruct->centroids[i] = -1;
  }
}


/*

*/
void makeCentroids(CentroidData_t *centroidStruct, int k, int dim)
{
  // allocate lists
  centroidStruct->groupID    = (int*)malloc(sizeof(int) * k);
  centroidStruct->sizes      = (int*)malloc(sizeof(int) * k);
  centroidStruct->coords     = (double*)malloc(sizeof(double) * k * dim);
  centroidStruct->prevCoords = (double*)malloc(sizeof(double) * k * dim);

  // check errors with mem allocation
  if (centroidStruct->groupID == NULL
      || centroidStruct->sizes == NULL
      || centroidStruct->coords == NULL
      || centroidStruct->prevCoords == NULL)
  {
    printf("Problem allocating memory [data_utils.c/makeCentroids]\n");
  }

  // assign values
  centroidStruct->k          = k;
  centroidStruct->dim        = dim;
}


/*

*/
void makeSaveOptions(SaveOptions_t *saveOptions)
{
  // allocate file path
  saveOptions->path = (char*)malloc(sizeof(char) * MAX_STR_BUFF_SIZE);

  // check errors with mem allocation
  if (saveOptions->path == NULL)
  {
    printf("Problem allocating memory [data_utils.c/makeSaveOptions]\n");
  }

  // reset flags
  saveOptions->options.outPoints = false;
  saveOptions->options.outCentroids = false;
  saveOptions->options.outTime = false;
}


/*

*/
void makeCentroidGroups(CentroidGroupData_t *groups, int p, int n)
{
  // allocate lists
  groups->centroidAss = (int*)malloc(sizeof(int) * p);
  groups->groupLcl    = (int*)malloc(sizeof(int) * p * n);
  groups->maxDrift    = (double*)malloc(sizeof(double) * p);

  // check errors with mem allocation
  if (groups->centroidAss == NULL
      || groups->groupLcl == NULL
      || groups->maxDrift == NULL)
      {
        printf("Problem allocating memory [data_utils.c/makeCentroidGroups]\n");
      }
}


/*

*/
void freePoints(PointData_t pointList, int n)
{
  // free pointList fields
  free(pointList.centroids);
  free(pointList.prevCentroids);
  free(pointList.coords);
  free(pointList.lb);
  free(pointList.ub);
}


/*

*/
void freeCentroids(CentroidData_t centroidList, int k)
{
  // free centroidList fields
  free(centroidList.groupID);
  free(centroidList.sizes);
  free(centroidList.coords);
  free(centroidList.prevCoords);
}


/*

*/
void freeSaveOptions(SaveOptions_t saveOptions)
{
  // free output path buffer
  free(saveOptions.path);
}


/*

*/
void freeCentroidGroups(CentroidGroupData_t groups)
{
  free(groups.centroidAss);
  free(groups.groupLcl);
  free(groups.maxDrift);
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
    centroidList->sizes[centrIdx] = 0;
  }
}


/*

*/
bool checkConvergence(CentroidData_t *centrList)
{
  for(int centrIdx = 0; centrIdx < centrList->k; centrIdx++)
  {
    for(int dimIdx = 0; dimIdx < centrList->dim; dimIdx++)
    {
      if(centrList->coords[centrIdx * centrList->dim + dimIdx]
        == centrList->prevCoords[centrIdx * centrList->dim + dimIdx]
        && !isnan(centrList->coords[centrIdx * centrList->dim + dimIdx]))
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
  for(int pointIdx = pointList->sublistOffset;
    pointIdx < pointList->sublistN + pointList->sublistOffset;
    pointIdx++)
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
    } /* end for */

    // update cluster membership
    pointList->centroids[pointIdx] = tempCentr;
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
      centrList->coords[pointList->centroids[pointIdx] * centrList->dim + dimIdx]
        += pointList->coords[pointIdx * pointList->dim + dimIdx];
    }

    // update number of points in cluster
    (centrList->sizes[pointList->centroids[pointIdx]])++;
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
      }
    }
  #endif
}


/*

*/
void updateCentroids_MPI(PointData_t *pointList, CentroidData_t *centrList,
                      int mpi_rank, int mpi_numProc, int rank_0_sublist_size,
                      int rank_non_0_sublist_size)
{
  /** begin processes divergence on rank **/

  // if not rank 0, send centroid assignments to rank 0, then wait for new centroid
  // locations
  if (mpi_rank != 0)
  {
    // send centroid assignments to rank 0
    MPI_Send(pointList->centroids + pointList->sublistOffset,
      pointList->sublistN,
      MPI_INTEGER,
      0, 0,
      MPI_COMM_WORLD);

    // receive new centroid locations from rank 0
    MPI_Status status;
    MPI_Recv(centrList->coords,
      centrList->k * centrList->dim,
      MPI_DOUBLE,
      0, 0,
      MPI_COMM_WORLD,
      &status);
  } /* end if mpi_rank != 0 */

  // if rank 0, recieve weighted sums, calculate new centroid locations,
  // and send locations back to all other ranks
  else
  {
    // receive centroid assignments
    MPI_Status status;
    int sendRecieveOffset;
    for (int i = 1; i < mpi_numProc; i++)
    {
      sendRecieveOffset = rank_0_sublist_size + ((i - 1) * rank_non_0_sublist_size);
      MPI_Recv(pointList->centroids + sendRecieveOffset,
        pointList->sublistN,
        MPI_INTEGER,
        i, 0,
        MPI_COMM_WORLD,
        &status);
    }

    // recalculate center of clusters
    updateCentroids(centrList, pointList);

    // send centroid locations
    for (int i = 1; i < mpi_numProc; i++)
    {
      sendRecieveOffset = (rank_0_sublist_size * pointList->dim)
          + ((i - 1) * rank_non_0_sublist_size);
      MPI_Send(centrList->coords,
        centrList->k * centrList->dim,
        MPI_DOUBLE,
        i, 0,
        MPI_COMM_WORLD);
    }
  } /* end if mpi_rank == 0 */

  /** end processes divergence on rank **/
}


/*

*/
void updateCentroids_yinyang(CentroidData_t *centrList, PointData_t *pointList,
                            CentroidGroupData_t *centrGrpList)
{

}
