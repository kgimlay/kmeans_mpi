// Author: Kevin Imlay

#include "../inc/command_line_utils.h"


/*
Command Line Arguments:
  Required: <data path> <data size> <data dim> <algo> <num cluster>
  Optional: -o <output path>
            -i <max iterations>
Required arguments first, order matters. Optional follows, order does not matter.
*/
bool parse_commandline(int argc, char *argv[], int *dataSize, int *dataDim,
  int *numClust, char *datasetFilePath, int *itrMax, char *outputFilePath,
  ALGO_CODE *algo_select)
{
  // operation variables
  char algo_buff[MAX_STR_BUFF_SIZE];

  // verify correct number of command line arguments
  if (argc < NUM_REQ_CMD_ARGS + 1)
  {
    printf("Too few command line arguments!\n\n%s\n\n", "Required: <data path>"\
      " <data size> <data dim> <algo> <cluster num>\n" \
      "Optional: -o <output path> -i <max iterations>");
    return false;
  }
  if (argc > NUM_REQ_CMD_ARGS + (NUM_OPT_CMD_ARGS * 2) + 1)
  {
    printf("Too many command line arguments!\n\n%s\n\n", "Required: <data path>"\
      " <data size> <data dim> <algo> <cluster num>\n" \
      "Optional: -o <output path> -i <max iterations>");
    return false;
  }
  if ((argc - NUM_REQ_CMD_ARGS) % 2 != 1)
  {
    printf("Incomplete optional arguments!\n\n%s\n\n", "Required: <data path>"\
      " <data size> <data dim> <algo> <cluster num>\n" \
      "Optional: -o <output path> -i <max iterations>");
    return false;
  }

  // parse command line arguments into return by parameter
  // required parameters
  sscanf(argv[1], "%s", datasetFilePath);       // data path
  sscanf(argv[2], "%d", dataSize);              // data size
  sscanf(argv[3], "%d", dataDim);               // data dim
  sscanf(argv[4], "%s", algo_buff);             // algorithm
  if (!str_to_algo(algo_buff, algo_select))
  {
    printf("Invalid algorithm provided!\n");
    return false;
  }
  sscanf(argv[5], "%d", numClust);              // num clusters

  // optional parameters
  for (int i = 0; i < argc - NUM_REQ_CMD_ARGS - 1; i += 2)
  {
    if (!strcmp(argv[NUM_REQ_CMD_ARGS + 1 + i], "-o"))  // output flag
    {
      sscanf(argv[NUM_REQ_CMD_ARGS + 2 + i], "%s", outputFilePath);
      // printf("%s\n", outputFilePath);
    }
    else if (!strcmp(argv[NUM_REQ_CMD_ARGS + 1 + i], "-i"))  // max iteration flag
    {
      sscanf(argv[NUM_REQ_CMD_ARGS + 2 + i], "%d", itrMax);
    }
  }

  // validate arguments
  /* TODO */

  // return success
  return true;
}
