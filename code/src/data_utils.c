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
