// Author: Kevin Imlay

#include "file_utils.h"

// Courtesy of Dr. Mike Gowanlock
int importDataset(char * fname, int DIM, int N, double ** dataset)
{
    FILE *fp = fopen(fname, "r");

    if (!fp) {
        printf("Unable to open file\n");
        return(1);
    }

    char buf[4096];
    int rowCnt = 0;
    int colCnt = 0;
    while (fgets(buf, 4096, fp) && rowCnt<N) {
        colCnt = 0;

        char *field = strtok(buf, ",");
        double tmp;
        sscanf(field,"%lf",&tmp);
        printf("%.4f\n", tmp);
        dataset[rowCnt][colCnt]=tmp;


        while (field) {
          colCnt++;
          field = strtok(NULL, ",");

          if (field!=NULL)
          {
          double tmp;
          sscanf(field,"%lf",&tmp);
          printf("%.4f\n", tmp);
          dataset[rowCnt][colCnt]=tmp;
          }

        }
        rowCnt++;
    }

    fclose(fp);
    return 0;
}


/*

*/
bool importDataPoints()
{
  return true;
}
