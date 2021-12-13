#!/bin/bash
#
#SBATCH --job-name=run_test_4
#SBATCH --output=/scratch/kgi4/run_test_4.txt
#SBATCH --chdir=/scratch/kgi4
#SBATCH --time=5:00
#SBATCH --mem=4000
#SBATCH --ntasks=1

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

module load openmpi
cd /home/kgi4/ondemand/data/sys/myjobs/projects/kmeans/

srun --cpu_bind=cores ./kmeans_main ./datasets/higgs_normalize_0_1.csv 11000000 28 SEQ_LLOYD 15 -v 3 -i 1000
