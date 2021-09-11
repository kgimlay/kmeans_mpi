// Author: Kevin Imlay

#include "lin_lloyd.h"


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
    free(centroidList[centrIdx].prevCoords);
    centroidList[centrIdx].prevCoords = centroidList[centrIdx].coords;
    centroidList[centrIdx].coords = (double *)calloc(centroidList[centrIdx].dim,
                                  sizeof(double));

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
void run_lin_lloyd(Point *pointList, int pointList_size, Centroid *centrList,
                    int centrList_size, int maxIter)
{
  // operation variables
  int iterationCntr;
  bool convergenceFlag = false;

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
        if(centrList[centrIdx].coords[dimIdx] != centrList[centrIdx].prevCoords[dimIdx])
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
}
