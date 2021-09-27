#!#!/usr/bin/env bash

MIN_K_POW=0
MAX_K_POW=2
K_POW_BASE=2
MAX_ITR=1
MIN_PROC=1
MAX_PROC=8
DATA_SET=M2.csv
DATA_POINTS=1000000
DATA_DIM=2


cd ../../.. # to root


# run sequential Lloyds
# for k in $(seq $MIN_K_POW $MAX_K_POW)
# do
#   k=$(($K_POW_BASE**$k))
#   echo 'Sequential Lloyds: k =' $k
#   for  _ in $(seq $MAX_ITR)
#   do
#     ./kmeans SEQ_LLOYD ./datasets/$DATA_SET $DATA_POINTS $DATA_DIM $k ./output/
#   done
#   echo ''
# done
# echo ''


# run mpi Lloyds
# for numP in $(seq $MIN_PROC $MAX_PROC)
# do
#   echo 'MPI Lloyds' $numP 'Process(es)'
#   for k in $(seq $MIN_K_POW $MAX_K_POW)
#   do
#     k=$(($K_POW_BASE**$k))
#     echo 'k =' $k
#     for _ in $(seq $MAX_ITR)
#     do
#       mpiexec -np $numP kmeans MPI_LLOYD ./datasets/$DATA_SET $DATA_POINTS $DATA_DIM $k ./output/
#     done
#     echo ''
#   done
#   echo ''
# done


# run sequential YinYang
for k in $(seq $MIN_K_POW $MAX_K_POW)
do
  k=$(($K_POW_BASE**$k))
  echo 'Sequential YinYang: k =' $k
  for  _ in $(seq $MAX_ITR)
  do
    ./kmeans SEQ_YINYANG ./datasets/$DATA_SET $DATA_POINTS $DATA_DIM $k ./output/
  done
  echo ''
done
echo ''


# run mpi YinYang
# for numP in $(seq $MIN_PROC $MAX_PROC)
# do
#   echo 'MPI YinYang' $numP 'Process(es)'
#   for k in $(seq $MIN_K_POW $MAX_K_POW)
#   do
#     k=$(($K_POW_BASE**$k))
#     echo 'k =' $k
#     for _ in $(seq $MAX_ITR)
#     do
#       mpiexec -np $numP kmeans MPI_YINYANG ./datasets/$DATA_SET $DATA_POINTS $DATA_DIM $k ./output/
#     done
#     echo ''
#   done
#   echo ''
# done
