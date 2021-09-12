#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *arv[])
{
  double **dataset = (double **)malloc(sizeof(double *) * 50);

  for(int i = 0; i < 50; i++) // dataset
  {
    printf("i: %d\n", i);
    dataset[i] = (double *)malloc(sizeof(double) * 2);
  }

  for(int i = 0; i < 50; i++)
  {
    for(int j = 0; j < 2; j++)
    {
      printf("i: %d, j: %d\n", i, j);
      dataset[i][j] = 0.0;
    }
  }
}
