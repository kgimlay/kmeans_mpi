// Author: Kevin Imlay

#include "../inc/kmeans_mpi_main.h"

/*

*/
int main(int argc, char *argv[])
{
  // operation variables
  ALGO_CODE algo_select;
  char dataFilePath_buff[MAX_STR_BUFF_SIZE];
  char outputFilePath_buff[MAX_STR_BUFF_SIZE];
  int dataSetSize = 0;
  int dataDimensionality = 0;
  int numClusters = 0;
  int maxIterations = DEFAULT_MAX_ITERATIONS;
  Point *dataPoints;
  Centroid *centroids;
  double **dataset;
  int mpi_numProc;
  int mpi_rank;

  // get command line arguments
  if (!parse_commandline(argc, argv, &algo_select, dataFilePath_buff, &dataSetSize,
    &dataDimensionality, &numClusters, &maxIterations, outputFilePath_buff))
  {
    printf("%s\n", "Terminating program.");
  }

  /* command line parsing successful, continue */
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

    /* File read successful */
    else
    {
      // make data points
      fillPoints(dataset, dataSetSize, dataDimensionality, dataPoints);
      // make centroids
      centroids = (Centroid *)malloc(sizeof(Centroid) * numClusters);
      makeCentroids(centroids, numClusters, dataDimensionality);

      /* BEGIN MPI SECTION */

      // init MPI and get rank and number of processes
      MPI_Init(&argc, &argv);
      MPI_Comm_size(MPI_COMM_WORLD, &mpi_numProc);
      MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

      // start the algorithm selected
      double sTime = MPI_Wtime();
      
      switch (algo_select) {
        case SEQ_LLOYD:
        {
          run_lin_lloyd(dataPoints, dataSetSize, centroids, numClusters,
                          maxIterations);
          break;
        }

        case MPI_LLOYD:
        {
          run_mpi_lloyd(dataPoints, dataSetSize, centroids, numClusters,
                          maxIterations, mpi_numProc, mpi_rank);
          break;
        }

        case SEQ_YINYANG:
        {
          run_lin_yin(dataPoints, dataSetSize, centroids, numClusters,
                          2, maxIterations);
          break;
        }

        case MPI_YINYANG:
        {
          run_mpi_yin(dataPoints, dataSetSize, centroids, numClusters,
                          maxIterations, mpi_numProc, mpi_rank);
          break;
        }

        default:
        {
          // should never get here!
          printf("Uh oh! [kmeans_mpi_main.c]\n");
          break;
        }
      }

      double eTime = MPI_Wtime();
      if (mpi_rank == 0)
      {
        printf("Runtime: %.6f seconds\n", eTime - sTime);
      }

      // end mpi
      MPI_Finalize();

      /* END MPI SECTION */

      // save results to files
      if(mpi_rank == 0)
      {
        if(exportResults(outputFilePath_buff, dataPoints, dataSetSize, centroids,
            numClusters)
            != FILE_OK)
        {
          printf("File could not be written!\n");
        }

        // report centroid locations
      //   printf("Centroids:\n");
      //   for (int i = 0; i < numClusters; i++)
      //   {
      //     printf("centroid %d: ", i);
      //     for (int j = 0; j < dataDimensionality; j++)
      //     {
      //       printf("%.3f, ", centroids[i].coords[j]);
      //     }
      //     printf("\n");
      //   }
      }

      // free memory
      freeCentroids(centroids, numClusters);
    }

    // free memory
    freeDataset(dataset, dataSetSize);
    freePoints(dataPoints, dataSetSize);

  } /* end else from command line arg parsing */

  return 0; // return successful operation
}
