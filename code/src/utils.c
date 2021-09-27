// Author: Keivn Imlay

#include "../inc/utils.h"

// strings
char *lin_lloyd_str = "SEQ_LLOYD\0";
char *mpi_lloyd_str = "MPI_LLOYD\0";
char *lin_yinyang_str = "SEQ_YINYANG\0";
char *mpi_yinyang_str = "MPI_YINYANG\0";


/*

*/
bool str_to_algo(char *input, ALGO_CODE *algo)
{
  // linear lloyd algo
  if (!strcmp(input, lin_lloyd_str)) {
    *algo = SEQ_LLOYD;
  }

  else if (!strcmp(input, mpi_lloyd_str))
  {
    *algo = MPI_LLOYD;
  }

  else if (!strcmp(input, lin_yinyang_str))
  {
    *algo = SEQ_YINYANG;
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
    strcpy(out, lin_lloyd_str);
  }

  // mpi lloyd algo
  else if (algo == MPI_LLOYD) {
    strcpy(out, mpi_lloyd_str);
  }

  // seq yinyang algo
  else if (algo == SEQ_LLOYD) {
    strcpy(out, lin_yinyang_str);
  }

  // catch all else, should never get here
  else {
    strcpy(out, "Uh Oh! [helper.c/algo_to_str]\0");
    return false;
  }

  return true;
}
