make        # make to reflect most recent changes
if [ ! $? -eq 0 ]; then echo 'build failed' && exit
fi
echo ''

./kmeans SEQ_YINYANG ./datasets/M2.csv 1000000 2 5 ./output/
