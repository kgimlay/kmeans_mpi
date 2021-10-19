// Author: Kevin Imlay

#include "../inc/mpi_yinyang.h"

void run_mpi_yin(PointData_t *pointList, CentroidData_t *centrList,
                  int maxIter, int mpi_numProc, int mpi_rank)
{
  printf("MPI Yinyang not implemented yet!\n");
}




// double startFullOnCPU(PointInfo *pointInfo,
//                     CentInfo *centInfo,
//                     DTYPE *pointData,
//                     DTYPE *centData,
//                     const int numPnt,
//                     const int numCent,
//                     const int numGrp,
//                     const int numDim,
//                     const int numThread,
//                     const int maxIter,
//                     unsigned int *ranIter)
// {
//   double startTime = omp_get_wtime();
//
//   // index variables
//   unsigned int pntIndex, grpIndex;
//   unsigned int index = 1;
//   unsigned int conFlag = 0;
//
//   // array to contain the maximum drift of each group of centroids
//   // note: shared amongst all points
//   DTYPE *maxDriftArr = (DTYPE *)malloc(sizeof(DTYPE) * numGrp);
//
//   // array of all the points lower bounds
//   DTYPE *pointLwrs = (DTYPE *)malloc(sizeof(DTYPE) * numPnt * numGrp);
//
//   // initiatilize to INFINITY
//   for(grpIndex = 0; grpIndex < numPnt * numGrp; grpIndex++)
//   {
//     pointLwrs[grpIndex] = INFINITY;
//   }
//
//   // array to contain integer flags which mark which groups need to be checked
//   // for a potential new centroid
//   // note: unique to each point
//   unsigned int *groupLclArr = (unsigned int *)malloc(sizeof(unsigned int)*numPnt*numGrp);
//
//   omp_set_num_threads(numThread);
//
//   // the minimum of all the lower bounds for a single point
//   DTYPE tmpGlobLwr = INFINITY;
//
//   // cluster the centroids into NGROUPCPU groups
//   groupCent(centInfo, centData, numCent, numGrp, numDim);
//
//   // run one iteration of standard kmeans for initial centroid assignments
//   initPoints(pointInfo, centInfo, pointData, pointLwrs,
//              centData, numPnt, numCent, numGrp, numDim, numThread);
//
//   // master loop
//   while(!conFlag && index < maxIter)
//   {
//     // clear drift array each new iteration
//     for(grpIndex = 0; grpIndex < numGrp; grpIndex++)
//     {
//       maxDriftArr[grpIndex] = 0.0;
//     }
//
//     // update centers via optimised update method
//     updateCentroids(pointInfo, centInfo, pointData, centData,
//                     maxDriftArr, numPnt, numCent, numGrp, numDim, numThread);
//
//     // filtering done in parallel
//     #pragma omp parallel \
//     private(pntIndex, grpIndex, tmpGlobLwr) \
//     shared(pointInfo, centInfo, pointData, centData, maxDriftArr, groupLclArr)
//     {
//       #pragma omp for schedule(static)
//       for(pntIndex = 0; pntIndex < numPnt; pntIndex++)
//       {
//         // reset old centroid before possibly finding a new one
//         pointInfo[pntIndex].oldCentroid = pointInfo[pntIndex].centroidIndex;
//
//         tmpGlobLwr = INFINITY;
//
//         // update upper bound
//           // ub = ub + centroid's drift
//         pointInfo[pntIndex].uprBound +=
//           centInfo[pointInfo[pntIndex].centroidIndex].drift;
//
//         // update group lower bounds
//           // lb = lb - maxGroupDrift
//         for(grpIndex = 0; grpIndex < numGrp; grpIndex++)
//         {
//           pointLwrs[(pntIndex * numGrp) + grpIndex] -= maxDriftArr[grpIndex];
//
//           if(pointLwrs[(pntIndex * numGrp) + grpIndex] < tmpGlobLwr)
//           {
//             // minimum lower bound
//             tmpGlobLwr = pointLwrs[(pntIndex * numGrp) + grpIndex];
//           }
//         }
//
//         // global filtering
//         // if global lowerbound >= upper bound
//         if(tmpGlobLwr < pointInfo[pntIndex].uprBound)
//         {
//           // tighten upperbound ub = d(x, b(x))
//           pointInfo[pntIndex].uprBound =
//             calcDisCPU(&pointData[pntIndex * numDim],
//                        &centData[pointInfo[pntIndex].centroidIndex * numDim],
//                        numDim);
//
//           // check condition again
//           if(tmpGlobLwr < pointInfo[pntIndex].uprBound)
//           {
//             // group filtering
//             for(grpIndex = 0; grpIndex < numGrp; grpIndex++)
//             {
//                 // mark groups that need to be checked
//               if(pointLwrs[(pntIndex * numGrp) + grpIndex] < pointInfo[pntIndex].uprBound)
//               groupLclArr[(pntIndex * numGrp) + grpIndex] = 1;
//               else
//               groupLclArr[(pntIndex * numGrp) + grpIndex] = 0;
//             }
//             // pass group array and point to go execute distance calculations
//             pointCalcsFullCPU(&pointInfo[pntIndex], centInfo,
//                               &pointData[pntIndex*numDim], &pointLwrs[pntIndex*numGrp],
//                               centData, maxDriftArr, &groupLclArr[pntIndex*numGrp],
//                               numPnt, numCent, numGrp, numDim);
//           }
//         }
//       }
//     }
//     index++;
//     conFlag = checkConverge(pointInfo, numPnt);
//   }
//   updateCentroids(pointInfo, centInfo, pointData, centData,
//                   maxDriftArr, numPnt, numCent, numGrp, numDim, numThread);
//   double endTime = omp_get_wtime();
//   *ranIter = index;
//
//   return endTime - startTime;
// }
