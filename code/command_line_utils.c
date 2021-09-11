// Author: Kevin Imlay

#include "command_line_utils.h"

// strings
char *numCoresFlag = "-c";
char *helpFlag = "-h";
char *manFlag = "-man";

char *helpStr = "\n"
"============================================================================\n"
"Command Line Arguments\n"
"Required:\n"
" <algorithm>\n"
"   LINLOYDS, LINLOYDS_MPI, YINYANG, YINYANG_MPI\n"
" <data filepath>                   (max: %d characters\n"
"   comma-delimited                 (preferrably csv)\n"
" <size of dataset>\n"
" <dimensionality of dataset>\n"
" <number of clusters>\n"
"\nOptional:\n"
" %s <number of cores>             (max: number of cores on machine)\n"
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
  int *numClusters)
{
  // operation variables
  char algo_buff[MAX_STR_BUFF_SIZE];

  // get data from argv
  if (argc == 6) {
    sscanf(argv[1], "%s", algo_buff);
    sscanf(argv[2], "%s", dataFilePath_buff);
    sscanf(argv[3], "%d", dataSetSize);
    sscanf(argv[4], "%d", dataDimensionality);
    sscanf(argv[5], "%d", numClusters);
  }
  else {
    printf("%s\n", "Incorrect number of arguments. Please run again with -h for"
    " help.");
    return false;
  }

  // validate required arguments
  if (!str_to_algo(algo_buff, algo)) {
    printf("%s is not a valid algorithm\n", algo_buff);
    return false;
  }
  if (*dataSetSize < 1) {
    printf("%s\n", "Dataset size must be at least 1");
    return false;
  }
  if (*dataDimensionality < 1) {
    printf("%s\n", "Data dimensionality must be at least 1");
    return false;
  }
  if (*numClusters < 1) {
    printf("%s\n", "Number of clusters must be at least 1");
    return false;
  }

  // success
  return true;
}


/*

*/
bool parseValidate_optional(int argc, char *argv[], int *numCores)
{
  // operation variables
  int tempNumCores = 0;

  // parse and validate everything after required
  for (int i = 6; i < argc; i++) {
    // number of cores
    if (!strcmp(argv[i], numCoresFlag) && ++i < argc) {
      sscanf(argv[++i], "%d", &tempNumCores);

      // number of cores must be at least 1
      if (tempNumCores < 1) {
        printf("%s\n", "Number of cores must be at least 1!");
        return false;
      }
      else if (tempNumCores <= MAX_CORES) {
        numCores = &tempNumCores;
      }
      // number of cores cannot exceed number in machine
      else {
        printf("%s\n", "Number of cores input exceeds the max of the machine!");
        return false;
      }
    }

    // catch all else
    else {
      printf("%s\n", " is not a valid flag!");
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
  int *numClusters, int *numCores)
{
  // first check for help flag or info flag
  for (int i = 0; i < argc; i++) {
    // help flag
    if (!strcmp(argv[i], helpFlag)) {
      printf(helpStr, MAX_STR_BUFF_SIZE, numCoresFlag, helpFlag, manFlag);
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
    dataSetSize, dataDimensionality, numClusters))
  {
    return false;
  }

  // parse optional arguments
  if (!parseValidate_optional(argc, argv, numCores))
  {
    return false;
  }

  // success
  return true;
}
