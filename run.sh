#!#!/usr/bin/env bash

# make        # make to reflect most recent changes
# if [ ! $? -eq 0 ]; then echo 'build failed' && exit
# fi
# echo ''

for i in $(seq 0 0)
do
  ./kmeans ./datasets/M.csv 1000000 2 SEQ_LLOYD 50 -s ./output/ -v 1
  mpiexec -np 8 ./kmeans ./datasets/M.csv 1000000 2 MPI_LLOYD 50 -s ./output/ -v 1
done
