#!/bin/bash
#
#SBATCH --job-name=run_test_1
#SBATCH --output=/scratch/kgi4/run_test_1.txt
#SBATCH --chdir=/scratch/kgi4
#SBATCH --time=0:10
#SBATCH --mem=500
#SBATCH --nodes=1
#SBATCH --ntasks=1

module load openmpi
cd /home/kgi4/ondemand/data/sys/myjobs/projects/kmeans/

srun ./kmeans_main ./datasets/M.csv 1000000 2 SEQ_LLOYD 15 -v 3 -i 1000
srun ./kmeans_main ./datasets/M.csv 1000000 2 SEQ_YINYANG 15 -v 3 -i 1000
