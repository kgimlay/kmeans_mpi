#!/bin/bash
#
#SBATCH --job-name=mpi_lloyd_susy_np32
#SBATCH --output=/scratch/kgi4/MPI_Lloyd/NP32/mpi_lloyd_susy_np32.txt
#SBATCH --chdir=/scratch/kgi4
#SBATCH --time=1:00:00
#SBATCH --mem=128000
#SBATCH --ntasks=32
#SBATCH --mail-type=ALL

DATASET=./datasets/SUSY_normalize_0_1.csv
N=5000000
D=18
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
