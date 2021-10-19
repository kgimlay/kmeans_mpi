#!#!/usr/bin/env bash

DATA_SET_PATH="./datasets/M.csv"
DATA_SIZE=1000000
DATA_DIM=2
MPI_NUMPROC=8
NUM_CENTROIDS=128
VERBOSITY=1
REPEAT_NUM=0
DO_BUILD=1

# build, if specified
if [ $DO_BUILD -eq 1 ];
then
  make        # make to reflect most recent changes
  if [ ! $? -eq 0 ]; then echo 'build failed' && exit
  fi
  echo ''
fi

# loop for number of times to repeat, or just go through once if no repeats
# specified
for i in $(seq 0 $REPEAT_NUM)
do

  # only print iteration number if repeating
  if [ $REPEAT_NUM -gt 0 ];
  then
    echo "Iteration" $i
  fi

  # sequential execution
  ./kmeans $DATA_SET_PATH $DATA_SIZE $DATA_DIM SEQ_LLOYD $NUM_CENTROIDS -s ./output/ -v $VERBOSITY
  if [ ! $? -eq 0 ]; then echo 'An error occurred! shutting down test\n' & exit
  fi

  # mpi execution
  mpiexec -np $MPI_NUMPROC ./kmeans $DATA_SET_PATH $DATA_SIZE $DATA_DIM MPI_LLOYD $NUM_CENTROIDS -s ./output/ -v $VERBOSITY
  if [ ! $? -eq 0 ]; then echo 'An error occurred! shutting down test\n' & exit
  fi
done
