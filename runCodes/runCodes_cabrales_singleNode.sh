#!/bin/bash

#SBATCH -N 1
#SBATCH --ntasks=72
#SBATCH --job-name=1005
##SBATCH --time=24:00:00
#SBATCH --mail-type=ALL
#SBATCH --error=job.%J.err
#SBATCH --output=job.%J.out

MAXlevel="12"
Ec="0e0"
We="15000"
Oh="3e-3"
tmax="200"

start=319


##for i in {0..11};
##do
##cd  $(printf "%03d" $((start+i)))
export OMP_NUM_THREADS=72
./dropAtomisation_test_v0 $MAXlevel $Ec $We $Oh $tmax 
##cd ../
##done
#CC99='mpicc -std=c99' qcc -Wall -O2 -D_MPI=1 burstingBubbleVE_ElasticLimit_v2_Snellius.c -o burstingBubbleVE_ElasticLimit_v2_Snellius -lm
