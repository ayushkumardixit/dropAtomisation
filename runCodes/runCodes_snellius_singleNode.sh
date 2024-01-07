#!/bin/bash

#SBATCH -N 1
#SBATCH --ntasks-per-node=64
#SBATCH --job-name=1008
#SBATCH --time=24:00:00
#SBATCH --mail-type=ALL
#SBATCH --error=job.%J.err
#SBATCH --output=job.%J.out
#SBATCH --mail-type=ALL
#SBATCH --mail-user=a.k.dixit@utwente.nl

MAXlevel="13"
Ec="0e0"
We="7.5"
Oh="3e-3"
tmax="200"

start=1008

#for i in 0;
#do
#cd $((start+i))
srun -n 64 --gres=cpu:64 --exclusive dropAtomisation_v1 $MAXlevel $Ec $We $Oh $tmax
#cd ../
#cd $((start+i+1))
#srun -n 64 --gres=cpu:64 --exclusive dropAtomisation_test_v0 $MAXlevel ${Ec[i+1]} $We $Oh $tmax & 
#cd ../
#wait
#done
#CC99='mpicc -std=c99' qcc -Wall -O2 -D_MPI=1 dropAtomisation_v1.c -o dropAtomisation_v1 -lm
