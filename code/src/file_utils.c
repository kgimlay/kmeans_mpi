// Author: Kevin Imlay

#include "../inc/file_utils.h"


/*

*/
FILE_CODE importCsv_double(double *dataset, int numRow, int numCol, char *fileName)
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
  for(rowCnt = 0; rowCnt < numRow; rowCnt++)
  {
    // get line and store in buffer
    if(fgets(lineBuffer, FILE_LINE_BUFF_SIZE, filePtr) == NULL)
    {
      return FILE_LINE_ERR;
    }

    // tokenize line to get data entries
    elemToken = strtok(lineBuffer, dataDelimiter);
    for(colCnt = 0; colCnt < numCol && elemToken != NULL; colCnt++)
    {
      // dataset[rowCnt][colCnt] = atof(elemToken); // figure out work around for return 0.0 if can't parse
      dataset[rowCnt * numCol + colCnt] = atof(elemToken); // figure out work around for return 0.0 if can't parse
      elemToken = strtok(NULL, dataDelimiter);
    }

  }

  // file import ok
  fclose(filePtr);
  return FILE_OK;
}


/*

*/
FILE_CODE exportCsv_double(double *outset, int numRow, int numCol, char *fileName)
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
      fprintf(filePtr, DOUBLE_EXP_PRECISION_FORMAT, outset[rowCnt * numCol + colCnt]);
      // printf("%.4f, ", outset[rowCnt * numCol + colCnt]);
      if(colCnt != numCol - 1)
      {
        fprintf(filePtr, ",");
      }
    }
    fprintf(filePtr, "\r\n");
    // printf("\n");
  }

  // file export ok
  fclose(filePtr);
  return FILE_OK;
}


/*

*/
FILE_CODE exportCsv_int(int *outset, int numRow, int numCol, char *fileName)
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
      fprintf(filePtr, "%d", outset[rowCnt * numCol + colCnt]);
      if(colCnt != numCol - 1)
      {
        fprintf(filePtr, ",");
      }
    }
    fprintf(filePtr, "\r\n");
  }

  // file export ok
  fclose(filePtr);
  return FILE_OK;
}
