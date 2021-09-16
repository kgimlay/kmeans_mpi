// Author: Kevin Imlay

#include "../inc/command_line_utils.h"

// strings
char *maxIterFlag = "-i";
char *helpFlag = "-h";
char *manFlag = "-man";

char *helpStr = "\n"
"============================================================================\n"
"Command Line Arguments\n\n"
"mpirun <MPI> kmeans <Required> <Optional>\n\n"
"MPI:\n"
" -nc <num processes>               (set to 1 for linear algorithms)\n"
"Required:\n"
" <algorithm>\n"
"   LIN_LLOYD, MPI_LLOYD, LIN_YINYANG, MPI_YINYANG\n"
" <data filepath>                   (max: %d characters)\n"
"   comma-delimited                 (a basic csv, not UTF-8 csv, etc.)\n"
" <size of dataset>\n"
" <dimensionality of dataset>\n"
" <number of clusters>              (less than data set size)\n"
" <output filepath>                 (max: %d characters)"
"\nOptional:\n"
" %s <max iterations>               (maximum number of iterations to allow)\n"
" %s                                (help. prints valid command line args)\n"
" %s                                (prints info on program and how to use)\n"
"============================================================================\n"
"\n";

char *manStr = "\n"
"============================================================================\n"
"Good luck! (to write later)\n"
"============================================================================\n"
"\n";


/*

*/
bool parseValidate_required(int argc, char *argv[], ALGO_CODE *algo,
  char *dataFilePath_buff, int *dataSetSize, int *dataDimensionality,
  int *numClusters, char *outputFilePath_buff)
{
  // operation variables
  char algo_buff[MAX_STR_BUFF_SIZE];

  // get data from argv
  if (argc >= 7) {
    sscanf(argv[1], "%s", algo_buff);
    sscanf(argv[2], "%s", dataFilePath_buff);
    sscanf(argv[3], "%d", dataSetSize);
    sscanf(argv[4], "%d", dataDimensionality);
    sscanf(argv[5], "%d", numClusters);
    sscanf(argv[6], "%s", outputFilePath_buff);
  }
  else {
    printf("%s\n", "\nIncorrect number of arguments. Please run again with -h for"
    " help.\n");
    return false;
  }

  // validate required arguments
  if (!str_to_algo(algo_buff, algo)) {
    printf("%s is not a valid algorithm\n", algo_buff);
    return false;
  }
  if (*dataSetSize < 1) {
    printf("Dataset size must be at least 1\n");
    return false;
  }
  if (*dataDimensionality < 1) {
    printf("Data dimensionality must be at least 1\n");
    return false;
  }
  if (*numClusters < 1) {
    printf("Number of clusters must be at least 1\n");
    return false;
  }
  if (*numClusters > *dataSetSize) {
    printf("Number of clusters must be at most the size of the data set\n");
    return false;
  }

  // success
  return true;
}


/*

*/
bool parseValidate_optional(int argc, char *argv[], int *numIterations)
{
  // parse and validate everything after required
  for (int i = 7; i < argc; i += 2) {
    // number of iterations
    if (!strcmp(argv[i], maxIterFlag) && i+1 < argc)
    {
      sscanf(argv[i + 1], "%d", numIterations);

      // number of iterations must be at least 1
      if (*numIterations < 0)
      {
        printf("Number of iterations must be at least 0!\n");
        return false;
      }
    }

    // catch all else
    else {
      printf("%s is not a valid flag!\n", argv[i]);
      return false;
    }
  }

  // success
  return true;
}



/*

*/
bool parse_commandline(int argc, char *argv[], ALGO_CODE *algo,
  char *dataFilePath_buff, int *dataSetSize, int *dataDimensionality,
  int *numClusters, int *numIterations, char *outputFilePath_buff)
{
  // first check for help flag or info flag
  for (int i = 0; i < argc; i++) {
    // help flag
    if (!strcmp(argv[i], helpFlag)) {
      printf(helpStr, MAX_STR_BUFF_SIZE, MAX_STR_BUFF_SIZE, maxIterFlag,
              helpFlag, manFlag);
      return false;
    }
    // manual flag
    else if (!strcmp(argv[i], manFlag)) {
      printf("%s", manStr);
      return false;
    }
  }

  // parse required arguments
  if (!parseValidate_required(argc, argv, algo, dataFilePath_buff,
    dataSetSize, dataDimensionality, numClusters, outputFilePath_buff))
  {
    return false;
  }

  // parse optional arguments
  if (!parseValidate_optional(argc, argv, numIterations))
  {
    return false;
  }

  // success
  return true;
}
