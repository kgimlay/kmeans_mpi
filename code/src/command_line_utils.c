// Author: Kevin Imlay

#include "../inc/command_line_utils.h"

const static char cmd_line_help_str_format[200] = "<Required> <Optional>\n" \
  "Required: <data path> <data size> <data dim> %s|%s|%s|%s <cluster num>\n" \
  "Optional: %s <max iterations> %s[%s,%s,%s] <output path>";

/*

*/
bool parse_commandline(int argc, char *argv[], int *dataSize, int *dataDim,
  int *numClust, char *datasetFilePath, int *itrMax,
  SaveOptions_t *fileOutputOptions, ALGO_CODE *algo_select)
{
  // operation variables
  char algo_buff[MAX_STR_BUFF_SIZE];
  char cmd_line_help_str[200];

  // format command line help string
  sprintf(cmd_line_help_str, cmd_line_help_str_format,
          SEQ_LLOYD__STR, MPI_LLOYD__STR, SEQ_YINYANG__STR, MPI_YINYANG__STR,
          MAX_ITR_FLAG, OUTPUT_FLAG, OUTPUT_PONT, OUTPUT_CENT, OUTPUT_TIME);

  // verify correct number of command line arguments
  if (argc < NUM_REQ_CMD_ARGS + 1)
  {
    printf("Too few command line arguments!\n\n%s\n\n", cmd_line_help_str);
    return false;
  }
  if (argc > NUM_REQ_CMD_ARGS + (NUM_OPT_CMD_ARGS * 2) + 1)
  {
    printf("Too many command line arguments!\n\n%s\n\n", cmd_line_help_str);
    return false;
  }
  if ((argc - NUM_REQ_CMD_ARGS) % 2 != 1)
  {
    printf("Incomplete optional arguments!\n\n%s\n\n", cmd_line_help_str);
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
    if (strstr(argv[NUM_REQ_CMD_ARGS + 1 + i], OUTPUT_FLAG))  // output flag
    {
      sscanf(argv[NUM_REQ_CMD_ARGS + 2 + i], "%s", fileOutputOptions->path);
      // check for arguments of what to output
      bool arg_provided_flg = false;
      if (strstr(argv[NUM_REQ_CMD_ARGS + 1 + i], OUTPUT_TIME))
      {
        arg_provided_flg = true;
        fileOutputOptions->options.outTime = true;
      }
      if (strstr(argv[NUM_REQ_CMD_ARGS + 1 + i], OUTPUT_CENT))
      {
        arg_provided_flg = true;
        fileOutputOptions->options.outCentroids = true;
      }
      if (strstr(argv[NUM_REQ_CMD_ARGS + 1 + i], OUTPUT_PONT))
      {
        arg_provided_flg = true;
        fileOutputOptions->options.outPoints = true;
      }
      // if no additional flag arguments provided, output all
      if (!arg_provided_flg)
      {
        fileOutputOptions->options.outTime = true;
        fileOutputOptions->options.outCentroids = true;
        fileOutputOptions->options.outPoints = true;
      }
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
