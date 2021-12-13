#!/bin/bash
#
#SBATCH --job-name=mpi_lloyd_higgs_np16
#SBATCH --output=/scratch/kgi4/MPI_Lloyd/NP16/mpi_lloyd_higgs_np16.txt
#SBATCH --chdir=/scratch/kgi4
#SBATCH --time=1:00:00
#SBATCH --mem=256000
#SBATCH --ntasks=16
#SBATCH --mail-type=ALL

DATASET=./datasets/higgs_normalize_0_1.csv
N=11000000
D=28
I=10000
V=1
ALG0=MPI_LLOYD
REPEAT=5

module load openmpi
cd /home/kgi4/ondemand/data/sys/myjobs/projects/kmeans/


# K=8
# echo "k=$K"
# for rep in $(seq $REPEAT)
# do
#   srun ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
# done
#
#
# K=32
# echo "k=$K"
# for rep in $(seq $REPEAT)
# do
#   srun ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
# done
#
#
# K=128
# echo "k=$K"
# for rep in $(seq $REPEAT)
# do
#   srun ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
# done
#
#
# K=512
# echo "k=$K"
# for rep in $(seq $REPEAT)
# do
#   srun ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
# done


K=1024
echo "k=$K"
for rep in $(seq $REPEAT)
do
  srun ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
done
