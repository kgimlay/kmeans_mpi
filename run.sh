echo 'starting mpi'
mpiexec -np 5 kmeans MPI_LLOYD ./datasets/M2.csv 1000000 2 5 ./output/
