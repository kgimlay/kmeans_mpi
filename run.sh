make        # make to reflect most recent changes
if [ ! $? -eq 0 ]; then echo 'build failed' && exit
fi
echo ''

./kmeans MPI_LLOYD ./datasets/M2.csv 1000000 2 5 ./output/
