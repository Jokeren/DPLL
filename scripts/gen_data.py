#!/bin/python

import os

P=[25,50,75,100,125,150,175,200,225,250,275,300,325,350,375,400,425,450,500,525,550,575,600,625,650,675,700,725,750,775]

for p in P:
    prefix = "extern/data/p" + str(p)
    os.system("mkdir -p " + prefix)
    r = 30
    while r <= 60:
        c = r * p / 10
        output = prefix + "/p" + str(p) + "c" + str(c) + "l3.cnf"
        cmd = "./tiny_sat -o " + output + " -p " + str(p) + " -c " + str(c) + " -l 3"
        print(cmd)
        os.system(cmd)
        r += 2
