// Author: Kevin Imlay

#include "../inc/file_utils.h"


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
FILE_CODE exportResults(char *outDir, Point *pointList, int size, Centroid *centroids,
  int numCentroids)
{
  // operation variables
  double **assTable = (double **)malloc(sizeof(double) * size);
  double **clustTable = (double **)malloc(sizeof(double) * numCentroids);
  char *outClustAss = (char *)malloc(sizeof(char) * MAX_STR_BUFF_SIZE);
  strcpy(outClustAss, outDir);
  strncat(outClustAss, "point_assignment.csv", MAX_STR_BUFF_SIZE);
  char *outCentr = (char *)malloc(sizeof(char) * MAX_STR_BUFF_SIZE);
  strcpy(outCentr, outDir);
  strncat(outCentr, "clusters.csv", MAX_STR_BUFF_SIZE);

  // allocate point assignment and fill
  for(int i = 0; i < size; i++)
  {
    assTable[i] = (double *)malloc(sizeof(double) * 1);
    *assTable[i] = (double)(pointList[i].centroid->id);
  }
  // send to export
  // todo: handle file errors
  exportCsv(assTable, size, 1, outClustAss);

  // allocate clusters and fill
  for(int i = 0; i < numCentroids; i++)
  {
    clustTable[i] = (double *)malloc(sizeof(double) * centroids[0].dim);
    for(int j = 0; j < centroids[0].dim; j++)
    {
      clustTable[i][j] = centroids[i].coords[j];
    }
  }
  // send to export
  // todo: handle file errors
  exportCsv(clustTable, numCentroids, centroids[0].dim, outCentr);

  // file export ok
  return FILE_OK;
}
