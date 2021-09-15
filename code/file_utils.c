// Author: Kevin Imlay

#include "file_utils.h"


/*

*/
FILE_CODE importDataset(double **dataset, int dimension, int dataSize, char *fileName)
{
  // operation variables
  char lineBuffer[FILE_LINE_BUFF_SIZE];
  int rowCnt;
  int colCnt;
  FILE *filePtr;
  char *elemToken;
  char *dataDelimiter = ",";

  // open file
  filePtr = fopen(fileName, "r");
  if(!filePtr) // file could not be opened
  {
    return FILE_OPEN_ERR;
  }

  // loop over lines of file
  for(rowCnt = 0; rowCnt < dataSize; rowCnt++)
  {
    // get line and store in buffer
    if(fgets(lineBuffer, FILE_LINE_BUFF_SIZE, filePtr) == NULL)
    {
      return FILE_LINE_ERR;
    }

    // tokenize line to get data entries
    elemToken = strtok(lineBuffer, dataDelimiter);
    for(colCnt = 0; colCnt < dimension && elemToken != NULL; colCnt++)
    {
      dataset[rowCnt][colCnt] = atof(elemToken); // figure out work around for return 0.0 if can't parse
      elemToken = strtok(NULL, dataDelimiter);
    }

  }

  // file import ok
  return FILE_OK;
}


/*

*/
FILE_CODE exportCsv(double **outset, int numRow, int numCol, char *fileName)
{
  // operatin variables
  int rowCnt;
  int colCnt;
  FILE *filePtr;

  // open filePtr
  filePtr = fopen(fileName, "w");
  if(!filePtr) // file could not be opened
  {
    return FILE_OPEN_ERR;
  }

  // loop over rows of output set and print to the file
  for(rowCnt = 0; rowCnt < numRow; rowCnt++)
  {
    for(colCnt = 0; colCnt < numCol; colCnt++)
    {
      fprintf(filePtr, "%.14f", outset[rowCnt][colCnt]);
      if(colCnt != numCol - 1)
      {
        fprintf(filePtr, ",");
      }
    }
    fprintf(filePtr, "\r\n");
  }

  // file export ok
  return FILE_OK;
}


/*

*/
FILE_CODE exportResults(char *outDir)
{
  return FILE_OK;
}
