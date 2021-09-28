// Author: Keivn Imlay

#include "../inc/seq_yinyang.h"

void run_lin_yin(Point *pointList, int pointList_size, Centroid *centrList,
                    int centrList_size, int maxIter)
{
  // select starting points for centroids
  startCentroids(centrList, centrList_size, pointList, pointList_size);

  // group centroids into t groups


  // run sequential lloyd's for first iteration and assign lower and upper bounds


  // while no convergence and not at max iterations
  // note, counter starting at 1 because at this point starting at 2nd iteration
  for(int iterationCntr = 1; iterationCntr < maxIter; iterationCntr++)
  {
    // update centroids with optimized update method


    // prime centroids for next iteration


    // update upper bounds


    // update lower bounds


    // global filtering


    // check for convergence
    if (checkConvergence(centrList, centrList_size))
    {
      break;
    }

  } /* end for  */
}
