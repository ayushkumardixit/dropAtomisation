#!/bin/bash

id="02_v2.mp4"
#rm burstingBubbleVE_ElasticLimit
#qcc -fopenmp -Wall -O2 burstingBubbleVE_ElasticLimit_v2.c -o burstingBubbleVE_ElasticLimit -lm

# qcc -Wall -O2 burstingBubbleVE_ElasticLimit_v2.c -o burstingBubbleVE_ElasticLimit -lm

#export OMP_NUM_THREADS=10
#./burstingBubbleVE_ElasticLimit $MAXlevel $Ec $Oh $Bond $tmax


python3 VideoBurstingBubble_facets_v2_jetheight.py
cd Video
ffmpeg -framerate 35 -pattern_type glob -i '*.png' -vf "pad=ceil(iw/2)*2:ceil(ih/2)*2" -c:v libx264 -r 30 -pix_fmt yuv420p $id

#cd ../../006/
#bash runCodes.sh
