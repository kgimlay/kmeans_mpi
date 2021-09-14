// Author: Kevin Imlay

#ifndef FILE_UTILS_H
#define FILE_UTILS_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

// defines

// separate from MAX_STR_BUFF_SIZE
// max buffer size for reading file
#define FILE_LINE_BUFF_SIZE 4096

#define POINT_ASSIGNMENT_CSV "point_assignment.csv"

// enums

// return codes for file operations
typedef enum
{
  FILE_OK = 200,
  FILE_OPEN_ERR,
  FILE_LINE_ERR
} FILE_CODE;

// function prototypes
FILE_CODE importDataset(double **dataset, int dimension, int dataSize, char *fileName);
FILE_CODE exportCsv(double **outset, int numRow, int numCol, char *fileName);


#endif
