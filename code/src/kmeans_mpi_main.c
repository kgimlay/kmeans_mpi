// Author: Kevin Imlay

#include "../inc/kmeans_mpi_main.h"

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
  int maxIterations = DEFAULT_MAX_ITERATIONS;
  char *dataFilePath_buff = (char*)calloc(MAX_STR_BUFF_SIZE, sizeof(char));
  ALGO_CODE algo_select;
  CentroidData_t centroids;
  PointData_t points;
  SaveOptions_t sOptions;
  TimeData_t time;

  /* Init MPI */
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  // get command line arguments
  makeSaveOptions(&sOptions);
  if (!parse_commandline(argc, argv, &data_size, &data_dim,
    &num_clusters, dataFilePath_buff, &maxIterations, &sOptions, &algo_select))
  {
    printf("Command line parse failed, shutting down.\n");
    return 1;
  }

  // command line parsing successful, allocate memory for point and centroid data
  makeCentroids(&centroids, num_clusters, data_dim);
  makePoints(&points, data_size, data_dim);

  // import the dataset into the pointData struct
  importCsv_double(points.coords, data_size, data_dim, dataFilePath_buff);

  // start timing algo
  time.algoStartTime = wallTime();

  // setup complete, call algorithm for execution
  if (algo_select == SEQ_LLOYD)
  {
    run_seq_lloyd(&points, &centroids, maxIterations);
  }
  else if (algo_select == MPI_LLOYD)
  {
    run_mpi_lloyd(&points, &centroids, maxIterations, mpi_numProc, mpi_rank);
  }
  // else if (algo_select == SEQ_YINYANG)
  // {
  //   run_lin_yin(dataPoints, centroids, numClusters, maxIterations);
  // }
  // else if (algo_select == MPI_YINYANG)
  // {
  //   run_mpi_yin(dataPoints, centroids, maxIterations, mpi_numProc, mpi_rank);
  // }
  else
  {
    printf("Uh oh! Algo not available! [kmeans_mpi_main.c]\n");
    return 1;
  }

  // end timing algo
  time.algoEndTime = wallTime();

  // save results, if output specified
  if (mpi_rank == 0 && strlen(sOptions.path) != 0)
  {
    if (sOptions.options.outPoints)
    {
      // save point assignemnt
      char fileNamePath[MAX_STR_BUFF_SIZE];
      strcpy(fileNamePath, sOptions.path);
      strcat(fileNamePath, "point_assignment.csv");
      exportCsv_int(points.centroid, points.n, 1, fileNamePath);
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
      // save time information
      char fileNamePath[MAX_STR_BUFF_SIZE];
      strcpy(fileNamePath, sOptions.path);
      strcat(fileNamePath, "time_metrics.csv");
      double timeArr[1] = {deltaTime(time.algoStartTime, time.algoEndTime)};
      exportCsv_double(timeArr, 1, 1, fileNamePath);
    }
  }

  /* Deinit MPI */
  MPI_Finalize();

  // shut down
  freePoints(points, data_size);
  freeCentroids(centroids, num_clusters);
  freeSaveOptions(sOptions);
  free(dataFilePath_buff);

  return 0; // return successful operation
}
