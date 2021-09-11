// Author: Kevin Imlay

#include "kmeans_mpi_main.h"

/*

*/
int main(int argc, char *argv[])
{
  // operation variables
  ALGO_CODE *algo_select = (ALGO_CODE *)malloc(sizeof(ALGO_CODE));
  char dataFilePath_buff[MAX_STR_BUFF_SIZE];
  int dataSetSize = 0;
  int dataDimensionality = 0;
  int numClusters = 0;
  int numCores = MAX_CORES;
  int maxIterations = 1000;    // todo: add to command line as optional argument
  Point *dataPoints;
  Centroid *centroids;

  // get command line arguments
  if (!parse_commandline(argc, argv, algo_select, dataFilePath_buff, &dataSetSize,
    &dataDimensionality, &numClusters, &numCores))
  {
    printf("%s\n", "Terminating program.");
  }
  // command line parsing successful, continue
  else
  {
    // allocate memory for dataset and centroids
    centroids = (Centroid *)malloc(sizeof(Centroid) * numClusters);
    dataPoints = (Point *)malloc(sizeof(Point) * dataSetSize);

    // print arguments
#if VERBOSE==1
    char tempAlgoStr[MAX_STR_BUFF_SIZE];
    algo_to_str(*algo_select, tempAlgoStr);
    printf("\n======== Parameters =========\n"
      "Algorithm: %s\nDataset Filepath: %s\nDataset Size: %d\n"
      "Dimensionality: %d\nNumber Clusters: %d\nNumber Cores: %d\n\n",
      tempAlgoStr, dataFilePath_buff, dataSetSize, dataDimensionality,
      numClusters, numCores);
#endif /* end VERB==1 */

    // allocate data points and centroids
    // centroids
    for(int i = 0; i < numClusters; i++)
    {
      centroids[i].id = i;
      centroids[i].dim = dataDimensionality;
      centroids[i].coords = (double *)calloc(dataDimensionality, sizeof(double));
      centroids[i].prevCoords = (double *)calloc(dataDimensionality, sizeof(double));
      centroids[i].size = 0;
    }

    // data points
    for(int i = 0; i < dataSetSize; i++)
    {
      dataPoints[i] = *(Point *)malloc(sizeof(Point));
      dataPoints[i].id = i;
      dataPoints[i].dim = dataDimensionality;
      dataPoints[i].coords = (double *)calloc(dataDimensionality, sizeof(double));
      dataPoints[i].centroid = &centroids[i%numClusters];
    }

    // open dataset and fill data points
    // Courtesy of Dr. Mike Gowanlock
      //pointer to entire dataset
      double ** dataset;

      //allocate memory for dataset
      dataset=(double**)malloc(sizeof(double*)*dataSetSize);
      for (int i=0; i<dataSetSize; i++)
      {
        dataset[i]=(double*)malloc(sizeof(double)*dataDimensionality);
      }

      importDataset(dataFilePath_buff, dataDimensionality, dataSetSize, dataset);
    // End Courtesy of Dr. Mike Gowanlock
    // fill data points
    for(int i = 0; i < dataSetSize; i++)
    {
      for(int j = 0; j < dataDimensionality; j++)
      {
        dataPoints[i].coords[j] = dataset[i][j];
      }
    }
    printf("Dataset: \n");
    for (int i=0; i<dataSetSize; i++)
    {
      for (int j=0; j<dataDimensionality; j++)
      {
          printf("%f, ", dataPoints[i].coords[j]);
      }
      printf("\n");
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
    printf("Centroids: \n");
    for (int i=0; i<numClusters; i++)
    {
      for (int j=0; j<dataDimensionality; j++)
      {
          printf("%f, ", centroids[i].coords[j]);
      }
      printf("\n");
    }


    // start the algorithm selected
    switch (*algo_select) {
      case LINEAR_LLOYD:
        run_lin_lloyd(dataPoints, dataSetSize, centroids, numClusters, maxIterations);
        break;

      default:
        break;
    }

    // print results
#if VERBOSE==1
    printf("\n\nPoint ID  |  Cluster ID\n");
    for (int i = 0; i < dataSetSize; i++)
    {
      printf("   %d     %d\n", dataPoints[i].id, dataPoints[i].centroid->id);
    }
    printf("\nCluster ID  |  Coords\n");
    for (int i = 0; i < numClusters; i++)
    {
      printf("     %d    ", centroids[i].id);
      for (int j = 0; j < centroids[i].dim; j++)
      {
        printf("%.3f, ", centroids[i].coords[j]);
      }
      printf("\n");
    }
#endif /* end VERB==1 */

    // save results to files


    // free memory
    // dataPoints
    // centroids
  }
  // free memory
  free(algo_select);
}
