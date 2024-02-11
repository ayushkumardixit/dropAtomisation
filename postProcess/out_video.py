# Author: Vatsal Sanjay
# vatsalsanjay@gmail.com
# Physics of Fluids

import numpy as np
import subprocess as sp
import os
nGFS = 100 

def gettingview(filename,name):
    exe = ["./getView", filename, name]
    p = sp.Popen(exe, stdout=sp.PIPE, stderr=sp.PIPE)
    stdout, stderr = p.communicate()
    

folder = 'Video_view'  # output folder
if not os.path.isdir(folder):
    os.makedirs(folder)

for ti in range(nGFS):
    t = 2 * ti
    place = "intermediate/snapshot-%5.4f" % t
    name = "%s/%4.4d.png" %(folder, int(t))
    if not os.path.exists(place):
        print("File %s not found!" % place)
    else:
        if os.path.exists(name):
            print("Image %s found!" % name)
        else:
            gettingview(place,name)
            print(("Done %d of %d" % (ti, nGFS)))