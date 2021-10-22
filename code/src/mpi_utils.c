// Author: Kevin Imlay

#include "../inc/mpi_utils.h"


/*

*/
int calcPointSublistSize_rank0(int data_size, int num_ranks)
{
  return calcPointSublistSize_rankNon0(data_size, num_ranks)
    + data_size % num_ranks;
}


/*

*/
int calcPointSublistSize_rankNon0(int data_size, int num_ranks)
{
  return data_size / num_ranks;
}
