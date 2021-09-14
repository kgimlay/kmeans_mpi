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
    &dataDimensionality, &numClusters, &maxIterations, &numCores, outputFilePath_buff))
  {
    printf("%s\n", "Terminating program.");
  }
  // command line parsing successful, continue
  else
  {
    // allocate data
    centroids = (Centroid *)malloc(sizeof(Centroid) * numClusters);
    for(int i = 0; i < numClusters; i++)  // centroids
    {
      centroids[i].id = i;
      centroids[i].dim = dataDimensionality;
      centroids[i].coords = (double *)calloc(dataDimensionality, sizeof(double));
      centroids[i].prevCoords = (double *)calloc(dataDimensionality, sizeof(double));
      centroids[i].size = 0;
    }
    dataPoints = (Point *)malloc(sizeof(Point) * dataSetSize);
    for(int i = 0; i < dataSetSize; i++)  // data points
    {
      dataPoints[i] = *(Point *)malloc(sizeof(Point));
      dataPoints[i].id = i;
      dataPoints[i].dim = dataDimensionality;
      dataPoints[i].coords = (double *)calloc(dataDimensionality, sizeof(double));
      dataPoints[i].centroid = &centroids[i%numClusters];
    }
    dataset = (double **)malloc(sizeof(double *) * dataSetSize);
    for(int i = 0; i < dataSetSize; i++) // dataset
    {
      dataset[i] = (double *)malloc(sizeof(double) * dataDimensionality);
    }

    // import dataset from file
    if(importDataset(dataset, dataDimensionality, dataSetSize, dataFilePath_buff) != FILE_OK)
    {
      printf("File could not be read!\n");
    }
    else
    {
      // fill data points
      for(int i = 0; i < dataSetSize; i++)
      {
        for(int j = 0; j < dataDimensionality; j++)
        {
          dataPoints[i].coords[j] = dataset[i][j];
        }
      }

      // select starting points for centroids
      for(int i = 0; i < numClusters; i++)
      {
        for(int j = 0; j < dataDimensionality; j++)
        {
          // set the n centroid locations the first n data points
          centroids[i].coords[j] = dataPoints[i].coords[j];
        }
      }

      // start the algorithm selected
      switch (*algo_select) {
        case LINEAR_LLOYD:
          run_lin_lloyd(dataPoints, dataSetSize, centroids, numClusters, maxIterations);
          break;

        default:
          break;
      }

      // save results to files
      if(exportCsv(dataset, dataSetSize, dataDimensionality, outputFilePath_buff) != FILE_OK)
      {
        printf("File could not be written!\n");
      }
    }

    // free memory
    // dataset
    for(int i = 0; i < dataSetSize; i++)
    {
      free(dataset[i]);
    }
    free(dataset);

    // dataPoints
    for(int i = 0; i < dataSetSize; i++)  // data points
    {
      free(dataPoints[i].coords);
    }
    free(dataPoints);

    // centroids
    for(int i = 0; i < numClusters; i++)  // centroids
    {
      free(centroids[i].coords);
      free(centroids[i].prevCoords);
    }
    free(centroids);

  } /* end else from command line arg parsing */

  // free memory
  free(algo_select);
}
