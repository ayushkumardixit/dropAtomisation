#!/bin/bash

id="1018.mp4"

python3 VideoDropAtomisation_facets.py 
cd Video
ffmpeg -framerate 15 -pattern_type glob -i '*.png' -vf "pad=ceil(iw/2)*2:ceil(ih/2)*2" -c:v libx264 -r 30 -pix_fmt yuv420p $id

