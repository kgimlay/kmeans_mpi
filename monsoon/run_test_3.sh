#!/bin/bash
#
#SBATCH --job-name=run_test_3
#SBATCH --output=/scratch/kgi4/run_test_3.txt
#SBATCH --chdir=/scratch/kgi4
#SBATCH --time=0:30
#SBATCH --mem=2000
#SBATCH --cpus-per-task=8

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

module load openmpi
cd /home/kgi4/ondemand/data/sys/myjobs/projects/kmeans/

srun --cpu_bind=cores ./kmeans_main ./datasets/M.csv 1000000 2 MPI_OMP_LLOYD 15 -v 3 -i 1000
# srun --cpu_bind=cores ./kmeans_main ./datasets/M.csv 1000000 2 MPI_OMP_YINYANG 15 -v 3 -i 1000
