#!/bin/bash

MAXlevel="10"
Ec="0e0"
We="15000"
Oh="3e-3"
tmax="200"

rm dropAtomisation_test_v0 
qcc -fopenmp -Wall -O2 dropAtomisation_test_v0.c -o dropAtomisation_test_v0 -lm

# qcc -Wall -O2 burstingBubbleVE_ElasticLimit_v2.c -o burstingBubbleVE_ElasticLimit -lm

export OMP_NUM_THREADS=4
./dropAtomisation_test_v0 $MAXlevel $Ec $We $Oh $tmax
