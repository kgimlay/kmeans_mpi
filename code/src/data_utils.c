// Author: Kevin Imlay

#include "../inc/data_utils.h"


void makePoints(Point *pointList, int size, int dim)
{
  // allocate points
  for(int i = 0; i < size; i++)
  {
    pointList[i] = *(Point *)malloc(sizeof(Point));
    pointList[i].id = i;
    pointList[i].dim = dim;
    pointList[i].coords = (double *)calloc(dim, sizeof(double));
    pointList[i].centroid = NULL;
  }
}

void fillPoints(double **data, int size, int dim, Point *pointList)
{
  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < dim; j++)
    {
      pointList[i].coords[j] = data[i][j];
    }
  }
}


void makeCentroids(Centroid *centroidList, int num, int dim)
{
  // allocate centroids
  for(int i = 0; i < num; i++)
  {
    centroidList[i].id = i;
    centroidList[i].dim = dim;
    centroidList[i].coords = (double *)calloc(dim, sizeof(double));
    centroidList[i].prevCoords = (double *)calloc(dim, sizeof(double));
    centroidList[i].size = 0;
  }
}


void freePoints(Point *pointList, int num)
{
  for(int i = 0; i < num; i++)
  {
    free(pointList[i].coords);
  }
  free(pointList);
}


void freeCentroids(Centroid *centroidList, int num)
{
  for(int i = 0; i < num; i++)
  {
    free(centroidList[i].coords);
    free(centroidList[i].prevCoords);
  }
  free(centroidList);
}


void freeDataset(double **data, int num)
{
  for(int i = 0; i < num; i++)
  {
    free(data[i]);
  }
  free(data);
}


/*

*/
double calcSquaredEuclideanDist(Point point, Centroid centroid)
{
  // operaiton variables
  int dimensionality = point.dim;
  double tempSumSquared = 0.0;
  double tempPointDimCoord;
  double tempCentrDimCoord;

  // loop over each dimension
  for(int dimIdx = 0; dimIdx < dimensionality; dimIdx++)
  {
    // get dimensional coordinates
    tempPointDimCoord = point.coords[dimIdx];
    tempCentrDimCoord = centroid.prevCoords[dimIdx];

    // difference, square, and sum
    tempSumSquared += pow(tempPointDimCoord - tempCentrDimCoord, 2);
  }

  return tempSumSquared;
}



/*

*/
void primeCentroid(Centroid *centroidList, int listSize)
{
  // loop over each centroid
  for(int centrIdx = 0; centrIdx < listSize; centrIdx++)
  {
    // store current centroid pos into previous
    for(int dimIdx = 0; dimIdx < centroidList[centrIdx].dim; dimIdx++)
    {
      centroidList[centrIdx].prevCoords[dimIdx] = centroidList[centrIdx].coords[dimIdx];
      centroidList[centrIdx].coords[dimIdx] = 0.0;
    }

    // reset size for average calculation
    centroidList[centrIdx].size = 0;
  }
}



/*

*/
void updatePointClusterMembership(Point *pointList, int pointListSize,
                                  Centroid *centroidList, int centroidlistSize)
{
  // operation variables
  double tempMinDist;
  double tempDist;
  Centroid *tempCentr;

  // loop over each point
  for(int pointIdx = 0; pointIdx < pointListSize; pointIdx++)
  {
    tempMinDist = INFINITY;

    // loop over each centroid for distance calculation
    for(int centrIdx = 0; centrIdx < centroidlistSize; centrIdx++)
    {
      // calculate distance to each centroid
      tempDist = calcSquaredEuclideanDist(pointList[pointIdx], centroidList[centrIdx]);

      // store current minimum
      if(tempDist < tempMinDist)
      {
        tempCentr = &centroidList[centrIdx];
        tempMinDist = tempDist;
      }
    }

    // update cluster membership
    pointList[pointIdx].centroid = tempCentr;
  } /* end for */
}


/*

*/
void updateCentroids(Point *pointList, int pointListSize,
                      Centroid *centroidList, int centroidlistSize)
{
  // loop over each point
  for(int pointIdx = 0; pointIdx < pointListSize; pointIdx++)
  {
    // for each dimension, sum into centroid's coords
    for(int dimIdx = 0; dimIdx < pointList[pointIdx].dim; dimIdx++)
    {
      pointList[pointIdx].centroid->coords[dimIdx] += pointList[pointIdx].coords[dimIdx];
    }

    // update number of points in cluster
    pointList[pointIdx].centroid->size++;
  } /* end for */


  // loop over each centroid and average coords
  for(int centrIdx = 0; centrIdx < centroidlistSize; centrIdx++)
  {
    // average
    for(int dimIdx = 0; dimIdx < centroidList[centrIdx].dim; dimIdx++)
    {
      centroidList[centrIdx].coords[dimIdx] /= centroidList[centrIdx].size;
    }
  } /* end for */
}


/*

*/
void startCentroids(Centroid *centrList, int centrListSize,
                    Point *pointList, int pointListSize)
{
  // assign points to centroids, alternating
  // this helps with testing script; signifies disorder to begin
  for(int pointIdx = 0; pointIdx < pointListSize; pointIdx++)
  {
    pointList[pointIdx].centroid = &centrList[pointIdx%centrListSize];
  }

  /* select which method of setting starting positions for centroids */

  // first N datapoints
  #if CENTR_START_METHOD == 0
    // loop over centroids and pick datapoint n to set it's coords to
    for(int centrIdx = 0; centrIdx < centrListSize; centrIdx++)
    {
      // select point n
      Point *nPoint = &pointList[centrIdx];

      // set coords to coords of point n
      for(int dimIdx = 0; dimIdx < centrList[0].dim; dimIdx++)
      {
        centrList[centrIdx].coords[dimIdx] = nPoint->coords[dimIdx];
      }
    }
  #endif

  // spanning mean of alternating points
  #if CENTR_START_METHOD == 1
    // set centroid initial locations
    updateCentroids(pointList, pointListSize, centrList, centrListSize);
  #endif

  // random datapoints
  #if CENTR_START_METHOD == 2
    // random seed
    srand(RAND_SEED);

    // loop over centroids and pick a random datapoint to set it's coords to
    for(int centrIdx = 0; centrIdx < centrListSize; centrIdx++)
    {
      // get random point
      Point *randPoint = &pointList[rand() % pointListSize];

      // set coords to coords of a random point
      for(int dimIdx = 0; dimIdx < centrList[0].dim; dimIdx++)
      {
        centrList[centrIdx].coords[dimIdx] = randPoint->coords[dimIdx];
      }
    }
  #endif
  /* end starting position select */
}
