#!#!/usr/bin/env bash

./kmeans_wrapper ./datasets/M.csv 1000 2 SEQ_YINYANG 15 -v 3 -i 1000
./kmeans_wrapper ./datasets/M.csv 1000 2 MPI_YINYANG 15 -v 3 -i 1000 -np 8
