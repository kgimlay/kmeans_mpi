// Author: Kevin Imlay

#ifndef HELPER_H
#define HELPER_H


#include <string.h>
#include <stdbool.h>
#include "parameters.h"

// function prototypes
bool str_to_algo(char *input, ALGO_CODE *algo);
bool algo_to_str(ALGO_CODE algo, char *out);

#endif
