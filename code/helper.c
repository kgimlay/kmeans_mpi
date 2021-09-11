// Author: Keivn Imlay

#include "helper.h"

// strings
char *lin_lloyd_str = "LINEAR_LLOYD\0";


/*

*/
bool str_to_algo(char *input, ALGO_CODE *algo)
{
  // linear lloyd algo
  if (!strcmp(input, lin_lloyd_str)) {
    *algo = LINEAR_LLOYD;
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
bool algo_to_str(ALGO_CODE algo, char *out) {
  // linear llody algo
  if (algo == LINEAR_LLOYD) {
    strcpy(out, lin_lloyd_str);
  }

  // catch all else, should never get here
  else {
    strcpy(out, "Uh Oh! [helper.c/algo_to_str]\0");
    return false;
  }

  return true;
}
