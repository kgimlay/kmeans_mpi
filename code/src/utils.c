// Author: Keivn Imlay

#include "../inc/utils.h"


/*

*/
bool str_to_algo(char *input, ALGO_CODE *algo)
{
  // linear lloyd algo
  if (!strcmp(input, SEQ_LLOYD__STR)) {
    *algo = SEQ_LLOYD;
  }

  // mpi lloyd algo
  else if (!strcmp(input, MPI_LLOYD__STR))
  {
    *algo = MPI_LLOYD;
  }

  // mpi-omp lloyd algo
  else if (!strcmp(input, MPI_OMP_LLOYD__STR))
  {
    *algo = MPI_OMP_LLOYD;
  }

  // seq yinyang algo
  else if (!strcmp(input, SEQ_YINYANG__STR))
  {
    *algo = SEQ_YINYANG;
  }

  // mpi yinyang algo
  else if (!strcmp(input, MPI_YINYANG__STR))
  {
    *algo = MPI_YINYANG;
  }

  // mpi yinyang algo
  else if (!strcmp(input, MPI_OMP_YINYANG__STR))
  {
    *algo = MPI_OMP_YINYANG;
  }

  // catch all else
  else {
    algo = NULL;
    return false;
  }

  return true;
}


/*

*/
bool algo_to_str(ALGO_CODE algo, char *out)
{
  // linear lloyd algo
  if (algo == SEQ_LLOYD) {
    strcpy(out, SEQ_LLOYD__STR);
  }

  // mpi lloyd algo
  else if (algo == MPI_LLOYD) {
    strcpy(out, MPI_LLOYD__STR);
  }

  // seq yinyang algo
  else if (algo == SEQ_LLOYD) {
    strcpy(out, SEQ_YINYANG__STR);
  }

  // mpi yinyang algo
  else if (algo == MPI_LLOYD) {
    strcpy(out, MPI_YINYANG__STR);
  }

  // catch all else, should never get here
  else {
    strcpy(out, "Uh Oh! [helper.c/algo_to_str]\0");
    return false;
  }

  return true;
}
