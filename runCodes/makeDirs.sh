#!/bin/bash

start="1008"
end="1009"

for i in `seq $start $end`;
do
echo $i
mkdir -p $(printf "%03d" $i)
#scp -r *.c $i/
scp -r dropAtomisation_test_v0 get* VideoBurstingBubble*  $(printf "%03d" $i) 
#cp -r out_vol_time.py getdropstats* $i
#cp out_jheight* getXheight* $i
done
