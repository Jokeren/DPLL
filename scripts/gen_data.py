#!/bin/python

import os

P=[25,50,75,100,125,150,175,200,225,250]

for p in P:
    prefix = "extern/data/p" + str(p)
    os.system("mkdir -p " + prefix)
    r = 3.0
    while r < 6.0:
        c = int(r * p)
        r += 0.2
        output = prefix + "/p" + str(p) + "c" + str(c) + "l3.cnf"
        cmd = "./tiny_sat -o " + output + " -p " + str(p) + " -c " + str(c) + " -l 3"
        print(cmd)
        os.system(cmd)
