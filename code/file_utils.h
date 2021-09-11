// Author: Kevin Imlay

#ifndef FILE_UTILS_H
#define FILE_UTILS_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

// function prototypes
int importDataset(char * fname, int DIM, int N, double ** dataset);


#endif
