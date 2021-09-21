echo 'starting mpi'
mpiexec -np 3 kmeans MPI_LLOYD ./datasets/M2.csv 10000 2 5 ./output/
