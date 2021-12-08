#!#!/usr/bin/env bash

DATA_SET_PATH="./datasets/M.csv"
DATA_SET_PATH="./datasets/higgs_normalize_0_1.csv"
DATA_SIZE=1000000
DATA_DIM=2
MPI_NUMPROC=8
NUM_CENTROIDS=9
VERBOSITY=2
REPEAT_NUM=0
DO_BUILD=1
MAX_ITER=20

# build, if specified
# if [ $DO_BUILD -eq 1 ];
# then
#   make        # make to reflect most recent changes
#   if [ ! $? -eq 0 ]; then echo 'build failed' && exit
#   fi
#   echo ''
# fi

# loop for number of times to repeat, or just go through once if no repeats
# specified
for i in $(seq 0 $REPEAT_NUM)
do

  # only print iteration number if repeating
  if [ $REPEAT_NUM -gt 0 ];
  then
    echo "Iteration" $i
  fi

  #### sequential lloyd
  echo "Sequential Lloyd's"
  ./kmeans_main $DATA_SET_PATH $DATA_SIZE $DATA_DIM SEQ_LLOYD $NUM_CENTROIDS -s ./output/ -v $VERBOSITY -i $MAX_ITER
  if [ ! $? -eq 0 ]; then echo 'An error occurred! shutting down test\n' & exit
  fi

  #### mpi lloyd
  echo "MPI Lloyd's"
  mpiexec -np $MPI_NUMPROC ./kmeans_main $DATA_SET_PATH $DATA_SIZE $DATA_DIM MPI_LLOYD $NUM_CENTROIDS -s ./output/ -v $VERBOSITY -i $MAX_ITER
  if [ ! $? -eq 0 ]; then echo 'An error occurred! shutting down test\n' & exit
  fi

  #### sequential yinyang
  echo "Sequential YinYang"
  ./kmeans_main $DATA_SET_PATH $DATA_SIZE $DATA_DIM SEQ_YINYANG $NUM_CENTROIDS -s ./output/ -v $VERBOSITY -i $MAX_ITER
  if [ ! $? -eq 0 ]; then echo 'An error occurred! shutting down test\n' & exit
  fi

  #### mpi yinyang
  echo "MPI YinYang"
  mpiexec -np $MPI_NUMPROC ./kmeans_main $DATA_SET_PATH $DATA_SIZE $DATA_DIM MPI_YINYANG $NUM_CENTROIDS -s ./output/ -v $VERBOSITY -i $MAX_ITER
  if [ ! $? -eq 0 ]; then echo 'An error occurred! shutting down test\n' & exit
  fi

done
