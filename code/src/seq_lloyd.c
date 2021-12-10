// Author: Kevin Imlay

#include "../inc/seq_lloyd.h"


/*

*/
void run_seq_lloyd(PointData_t *pointList, CentroidData_t *centrList, int maxIter)
{
  // select starting points for centroids
  startCentroids(centrList, pointList);

  // while no convergence and not at max iterations
  for(int iterationCntr = 0; iterationCntr < maxIter; iterationCntr++)
  {
    // prime centroids for next iteration
    primeCentroid(centrList);

    // re-member points to clusters
    assignCentroids(pointList, centrList);

    // recalculate center of clusters
    updateCentroids(centrList, pointList);

    // printf("Centroids:\n");
    // for (int i = 0; i < centrList->k; i++)
    // {
    //   for (int j = 0; j < centrList->dim; j++)
    //   {
    //     printf("%.2f, ", centrList->coords[i * centrList->dim + j]);
    //   }
    //   printf("\n");
    // }
    // printf("\n");

    // check for convergence
    if (checkConvergence(centrList))
    {
      // printf("Iterations: %d\n", iterationCntr+1);
      break;
    }

  } /* end for  */
}
