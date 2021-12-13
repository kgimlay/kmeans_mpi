#!/bin/bash
#
#SBATCH --job-name=mpi_omp_lloyd_msd_np8
#SBATCH --output=/scratch/kgi4/MPI_OMP_Lloyd/NP8/mpi_omp_lloyd_msd_np8.txt
#SBATCH --chdir=/scratch/kgi4
#SBATCH --time=1:00:00
#SBATCH --mem=16000
#SBATCH --cpus-per-task=4
#SBATCH --ntasks=8
#SBATCH --mail-type=ALL

DATASET=./datasets/MSD_year_prediction_normalize_0_1.csv
N=515345
D=90
I=10000
V=1
ALG0=MPI_OMP_YINYANG
REPEAT=5

module load openmpi
cd /home/kgi4/ondemand/data/sys/myjobs/projects/kmeans/

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK


K=8
echo "k=$K"
for rep in $(seq $REPEAT)
do
  srun --cpu_bind=cores ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
done


K=32
echo "k=$K"
for rep in $(seq $REPEAT)
do
  srun --cpu_bind=cores ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
done


K=128
echo "k=$K"
for rep in $(seq $REPEAT)
do
  srun --cpu_bind=cores ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
done


K=512
echo "k=$K"
for rep in $(seq $REPEAT)
do
  srun --cpu_bind=cores ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
done


K=1024
echo "k=$K"
for rep in $(seq $REPEAT)
do
  srun --cpu_bind=cores ./kmeans_main $DATASET $N $D $ALG0 $K -v $V -i $I
done
