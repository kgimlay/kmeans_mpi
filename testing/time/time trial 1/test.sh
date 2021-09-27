#!#!/usr/bin/env bash

cd ../../.. # to root


# run sequential lloyds
for k in 1 2 4 8 16 32 64 128 256 512 1024
do
  echo 'Lin Lloyds: k =' $k
  for  itr in 1 2 3 4 5 6 7 8
  do
    ./kmeans LIN_LLOYD ./datasets/M2.csv 1000000 2 $k ./output/
  done
  echo ''
done
echo ''


# run mpi
for numP in 1 2 3 4 5 6 7 8
do
  echo 'MPI Lloyds' $numP 'Process(es)'
  for k in 1 2 4 8 16 32 64 128 256 512 1024
  do
    echo 'k =' $k
    for itr in 1 2 3 4 5 6 7 8
    do
      mpiexec -np $numP kmeans MPI_LLOYD ./datasets/M2.csv 1000000 2 $k ./output/
    done
    echo ''
  done
  echo ''
done
