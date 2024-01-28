# To get length of droplet with time

import numpy as np
import os
import subprocess as sp
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
from matplotlib.ticker import StrMethodFormatter
import sys

matplotlib.rcParams['font.family'] = 'serif'
matplotlib.rcParams['text.usetex'] = True
# matplotlib.rcParams['text.latex.preamble'] = [r'']

def gettinglength(filename):
    exe = ["./getlength", filename]
    p = sp.Popen(exe, stdout=sp.PIPE, stderr=sp.PIPE)
    stdout, stderr = p.communicate()
    temp1 = stderr.decode("utf-8")
    temp2 = temp1.split("\n")
    temp3 = temp2[0].split(" ")
    return float(temp3[0]), float(temp3[1]), float(temp3[2])
# ----------------------------------------------------------------------------------------------------------------------

nGFS = 5000
Ldomain = 4
GridsPerR = 64
nr = int(GridsPerR*Ldomain)

Jetheight_max_temp = -30.0
tsnap = 0.5

for ti in range(0, nGFS):
    t = tsnap*ti
    place = "intermediate/snapshot-%5.4f" % t

    if not os.path.exists(place):
        print("%s File not found!" % place)
    else:
         ttemp, xmintemp, xmaxtemp = gettinglength(place)
         print(ttemp)
         ltemp = xmaxtemp-xmintemp
         print("The length of droplet is %f" % ltemp)
         f = open("out_length_time.txt", "a")
         f.write("%4.6f %4.6f\n" % (ttemp, ltemp))

f.close()
        
         
         