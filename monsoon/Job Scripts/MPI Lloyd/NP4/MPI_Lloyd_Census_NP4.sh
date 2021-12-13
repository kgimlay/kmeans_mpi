#!/bin/bash
#
#SBATCH --job-name=mpi_lloyd_census_np4
#SBATCH --output=/scratch/kgi4/MPI_Lloyd/NP4/mpi_lloyd_census_np4.txt
#SBATCH --chdir=/scratch/kgi4
#SBATCH --time=1:00:00
#SBATCH --mem=16000
#SBATCH --ntasks=4
#SBATCH --mail-type=ALL

DATASET=./datasets/USCensus1990.data_no_first_col.csv
N=2458285
D=68
I=10000
V=1
ALG0=MPI_LLOYD
REPEAT=5

module load openmpi
cd /home/kgi4/ondemand/data/sys/myjobs/projects/kmeans/


K=8
echo "k=$K"
for rep in $(seq $REPEAT)
do
  srun ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
done


K=32
echo "k=$K"
for rep in $(seq $REPEAT)
do
  srun ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
done


K=128
echo "k=$K"
for rep in $(seq $REPEAT)
do
  srun ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
done


K=512
echo "k=$K"
for rep in $(seq $REPEAT)
do
  srun ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
done


K=1024
echo "k=$K"
for rep in $(seq $REPEAT)
do
  srun ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
done
