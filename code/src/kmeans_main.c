// Author: Kevin Imlay

#include "../inc/kmeans_main.h"

/*

*/
int main(int argc, char *argv[])
{
  // operation variables
  int mpi_numProc;
  int mpi_rank;
  int data_size;
  int data_dim;
  int num_clusters;
  int num_cluster_groups = 1;
  int verbose_level = DAFAULT_VERB_LEVEL;
  int maxIterations = DEFAULT_MAX_ITERATIONS;
  char *dataFilePath_buff = (char*)calloc(MAX_STR_BUFF_SIZE, sizeof(char));
  ALGO_CODE algo_select;
  CentroidData_t centroids;
  PointData_t points;
  SaveOptions_t sOptions;
  TimeData_t timeMetrics;

  /* Init MPI */
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  // get command line arguments
  makeSaveOptions(&sOptions);
  if (!parse_commandline(argc, argv, &data_size, &data_dim, &num_clusters,
    dataFilePath_buff, &maxIterations, &verbose_level, &sOptions, &algo_select))
  {
    printf("Command line parse failed, shutting down.\n");
    return 1;
  }

  // command line parsing successful, allocate memory for point and centroid data
  makeCentroids(&centroids, num_clusters, data_dim);
  makePoints(&points, data_size, data_dim, num_cluster_groups);

  // import the dataset into the pointData struct
  importCsv_double(points.coords, data_size, data_dim, dataFilePath_buff);

  // start timing algo
  timeMetrics.algoStartTime = wallTime();

  // setup complete, call algorithm for execution
  if (algo_select == SEQ_LLOYD)
  {
    run_seq_lloyd(&points, &centroids, maxIterations);
  }
  else if (algo_select == MPI_LLOYD)
  {
    run_mpi_lloyd(&points, &centroids, maxIterations, mpi_numProc, mpi_rank);
  }
  else if (algo_select == SEQ_YINYANG)
  {
    run_seq_yin(&points, &centroids, num_cluster_groups, maxIterations);
  }
  else if (algo_select == MPI_YINYANG)
  {
    run_mpi_yin(dataPoints, centroids, maxIterations, mpi_numProc, mpi_rank);
  }
  else
  {
    printf("Uh oh! Algo not available! [kmeans_mpi_main.c]\n");
    return 1;
  }

  // end timing algo
  timeMetrics.algoEndTime = wallTime();

  // save results, if output specified
  if (mpi_rank == 0 && strlen(sOptions.path) != 0)
  {
    if (sOptions.options.outPoints)
    {
      // save point assignemnt
      char fileNamePath[MAX_STR_BUFF_SIZE];
      strcpy(fileNamePath, sOptions.path);
      strcat(fileNamePath, "point_assignment.csv");
      exportCsv_int(points.centroids, points.n, 1, fileNamePath);
    }
    if (sOptions.options.outCentroids)
    {
      // save centroid coords
      char fileNamePath[MAX_STR_BUFF_SIZE];
      strcpy(fileNamePath, sOptions.path);
      strcat(fileNamePath, "centroid_coords.csv");
      exportCsv_double(centroids.coords, centroids.k, centroids.dim, fileNamePath);
    }
    if (sOptions.options.outTime)
    {
      // save timeMetrics information
      char fileNamePath[MAX_STR_BUFF_SIZE];
      strcpy(fileNamePath, sOptions.path);
      strcat(fileNamePath, "timeMetrics_metrics.csv");
      double timeMetricsArr[1] = {deltaTime(timeMetrics.algoStartTime, timeMetrics.algoEndTime)};
      exportCsv_double(timeMetricsArr, 1, 1, fileNamePath);
    }
  }

  // report results
  if (mpi_rank == 0)
  {
    // verbose level 1
    if (verbose_level >= 1)
    {
      printf("\n\n");
      printf("Algo Time: %.4f\n\n", deltaTime(timeMetrics.algoStartTime, timeMetrics.algoEndTime));
    }

    // verbose level 2
    if (verbose_level >= 2)
    {
      printf("Centroids:\n");
      for (int i = 0; i < centroids.k; i++)
      {
        for (int j = 0; j < centroids.dim; j++)
        {
          printf("%.2f, ", centroids.coords[i * centroids.dim + j]);
        }
        printf("\n");
      }
      printf("\n");
    }
    printf("\n");

    // verbose level 3
    // TODO: more verbosity levels
  }

  /* Deinit MPI */
  MPI_Finalize();

  // shut down
  freePoints(points);
  freeCentroids(centroids);
  freeSaveOptions(sOptions);
  free(dataFilePath_buff);

  return 0; // return successful operation
}
