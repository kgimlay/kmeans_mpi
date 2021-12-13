#!/bin/bash
#
#SBATCH --job-name=seq_lloyd_msd
#SBATCH --output=/scratch/kgi4/Sequential_Lloyd/seq_lloyd_msd.txt
#SBATCH --chdir=/scratch/kgi4
#SBATCH --time=1:00:00
#SBATCH --mem=4000
#SBATCH --ntasks=1
#SBATCH --mail-type=ALL

DATASET=./datasets/MSD_year_prediction_normalize_0_1.csv
N=515345
D=90
I=10000
V=1
ALG0=SEQ_LLOYD
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
