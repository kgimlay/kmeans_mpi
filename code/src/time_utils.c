// Author: Kevin Imlay

#include "../inc/time_utils.h"

/*

*/
double wallTime()
{
  return MPI_Wtime();
}


/*

*/
double deltaTime(double timeStart, double timeEnd)
{
  return timeEnd - timeStart;
}
