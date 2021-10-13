#!#!/usr/bin/env bash

# make        # make to reflect most recent changes
# if [ ! $? -eq 0 ]; then echo 'build failed' && exit
# fi
# echo ''

for i in $(seq 0 100)
do
  echo $i
  ./kmeans ./datasets/M2.csv 100 2 SEQ_LLOYD 5 ./output/
done
