// Author: Kevin Imlay

#include "kmeans_mpi_main.h"

/*

*/
int main(int argc, char *argv[])
{
  // operation variables
  ALGO_CODE *algo_select = (ALGO_CODE *)malloc(sizeof(ALGO_CODE));
  char dataFilePath_buff[MAX_STR_BUFF_SIZE];
  char outputFilePath_buff[MAX_STR_BUFF_SIZE];
  int dataSetSize = 0;
  int dataDimensionality = 0;
  int numClusters = 0;
  int numCores = MAX_CORES;
  int maxIterations = DEFAULT_MAX_ITERATIONS;
  Point *dataPoints;
  Centroid *centroids;
  double **dataset;

  // get command line arguments
  if (!parse_commandline(argc, argv, algo_select, dataFilePath_buff, &dataSetSize,
    &dataDimensionality, &numClusters, &maxIterations, &numCores,
    outputFilePath_buff))
  {
    printf("%s\n", "Terminating program.");
  }
  // command line parsing successful, continue
  else
  {
    // import dataset from file
    dataPoints = (Point *)malloc(sizeof(Point) * dataSetSize);
    makePoints(dataPoints, dataSetSize, dataDimensionality);
    dataset = (double **)malloc(sizeof(double *) * dataSetSize);
    for(int i = 0; i < dataSetSize; i++)
    {
      dataset[i] = (double *)malloc(sizeof(double) * dataDimensionality);
    }
    if(importDataset(dataset, dataDimensionality, dataSetSize, dataFilePath_buff)
        != FILE_OK)
    {
      printf("File could not be read!\n");
    }
    else
    {
      // make data points
      fillPoints(dataset, dataSetSize, dataDimensionality, dataPoints);
      // make centroids
      centroids = (Centroid *)malloc(sizeof(Centroid) * numClusters);
      makeCentroids(centroids, numClusters, dataDimensionality);

      // start the algorithm selected
      switch (*algo_select) {
        case LINEAR_LLOYD:
          run_lin_lloyd(dataPoints, dataSetSize, centroids, numClusters,
                          maxIterations);
          break;

        default:
          break;
      }

      // save results to files
      if(exportResults(outputFilePath_buff, dataPoints, dataSetSize, centroids,
          numClusters)
          != FILE_OK)
      {
        printf("File could not be written!\n");
      }

      // free memory
      freeCentroids(centroids, numClusters);
    }

    // free memory
    freeDataset(dataset, dataSetSize);
    freePoints(dataPoints, dataSetSize);

  } /* end else from command line arg parsing */

  // free memory
  free(algo_select);
}
