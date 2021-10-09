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
  int num_groups;
  int maxIterations = DEFAULT_MAX_ITERATIONS;
  char *dataFilePath_buff = (char*)calloc(MAX_STR_BUFF_SIZE, sizeof(char));
  char *outputFilePath_buff = (char*)calloc(MAX_STR_BUFF_SIZE, sizeof(char));
  ALGO_CODE algo_select;
  CentroidData_t *centroids;
  PointData_t *points;

  /* Init MPI */
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_numProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  // get command line arguments
  if (!parse_commandline(argc, argv, &data_size, &data_dim,
    &num_clusters, dataFilePath_buff, &maxIterations, outputFilePath_buff,
    &algo_select))
  {
    printf("Command line parse failed, shutting down.\n");
    return 1;
  }

  // command line parsing successful, allocate memory for point and centroid data
  centroids = (CentroidData_t*)malloc(sizeof(CentroidData_t) * data_size);
  points = (PointData_t*)malloc(sizeof(PointData_t) * num_clusters);
  makeCentroids(centroids, num_clusters, data_dim);
  makePoints(points, data_size, data_dim);

  // import the dataset into the pointData struct
  importDataset(points->coords, data_size, data_dim, dataFilePath_buff);

  // setup complete, call algorithm for execution
  if (algo_select == SEQ_LLOYD)
  {
    run_seq_lloyd(dataPoints, data_size, centroids, num_groups, maxIterations);
  }
  // else if (algo_select == MPI_LLOYD)
  // {
  //   run_mpi_lloyd(dataPoints, dataSetSize, centroids, numClusters,
  //                   maxIterations, mpi_numProc, mpi_rank);
  // }
  // else if (algo_select == SEQ_YINYANG)
  // {
  //   run_lin_yin(dataPoints, dataSetSize, centroids, numClusters,
  //                   3, maxIterations);
  // }
  // else if (algo_select == MPI_YINYANG)
  // {
  //   run_mpi_yin(dataPoints, dataSetSize, centroids, numClusters,
  //                   maxIterations, mpi_numProc, mpi_rank);
  // }
  else
  {
    printf("Uh oh! [kmeans_mpi_main.c]\n");
  }

  // save results, if output specified
  if (strlen(outputFilePath_buff) != 0)
  {
    // TODO: file output
  }

  /* Deinit MPI */
  MPI_Finalize();

  // shut down
  freePoints(points, data_size);
  freeCentroids(centroids, num_clusters);
  free(dataFilePath_buff);
  free(outputFilePath_buff);
  return 0; // return successful operation
}
