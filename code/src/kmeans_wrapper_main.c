// Author: Kevin Imlay

#include "../inc/kmeans_wrapper_main.h"

/*

*/
int main(int argc, char *argv[])
{
  // operation variables
  int numMpiProc;
  char numProcFlag_buff[MAX_STR_BUFF_SIZE];
  char algo_buff[MAX_STR_BUFF_SIZE];
  char sysCallSeq_buff[MAX_STR_BUFF_SIZE] = "./kmeans_main";
  char sysCallMpi_call[MAX_STR_BUFF_SIZE] = "mpiexec -np %d ./kmeans_main";
  char sysCallMpi_buff[MAX_STR_BUFF_SIZE];
  ALGO_CODE algo_select;


  // parse command line arguments to get algorithm being used
  sscanf(argv[4], "%s", algo_buff);
  if (!str_to_algo(algo_buff, &algo_select))
  {
    printf("Invalid algorithm provided!\n");
    return 1;
  }


  // if algoritm is non-mpi
  if (algo_select == SEQ_LLOYD || algo_select == SEQ_YINYANG)
  {
    // assemble the arguments array into a system call to kmeans main program
    for (int i = 1; i < argc; i++)
    {
      strcat(sysCallSeq_buff, " ");
      strcat(sysCallSeq_buff, argv[i]);
    }

    return system(sysCallSeq_buff);
  }
  // else, algorithm is mpi
  else
  {
    // parse command line arguments for num processes flag '-np'
    sscanf(argv[argc-2], "%s", numProcFlag_buff);
    if (!strcmp(numProcFlag_buff, "-np"))
    {
      sscanf(argv[argc-1], "%d", &numMpiProc);
    }
    else
    {
      printf("No '-np' flag provided!\n");
      return 1;
    }

    // assemble the arguments array into a system call to kmeans main program
    sprintf(sysCallMpi_buff, sysCallMpi_call, numMpiProc);
    for (int i = 1; i < argc - 2; i++)
    {
      strcat(sysCallMpi_buff, " ");
      strcat(sysCallMpi_buff, argv[i]);
    }

    return system(sysCallMpi_buff);
  }


  // possibly in the future this can add testing functionality and functionality
  // for automating statistics calculations

}
