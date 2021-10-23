// Author: Kevin Imlay

#include "../inc/kmeans_wrapper_main.h"

/*

*/
int main(int argc, char *argv[])
{
  int status = system("mpiexec -np 8 ./kmeans_main ./datasets/M.csv 1000000 2 MPI_LLOYD 10 -v 2");

  return 0; // return success
}
