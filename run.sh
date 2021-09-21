echo 'starting mpi'
mpiexec -np 8 kmeans MPI_LLOYD ./datasets/M2.csv 1000000 2 5 ./output/
