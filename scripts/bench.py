#!/bin/python

import subprocess, threading
import os
import sys

def run(cmd, calls=None, times=None, timeout=60.0):
    process = None
    def target():
        process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        stdout, stderr = process.communicate()
        if calls is not None:
            if stdout.find("-->") != -1:
                split = stdout.split("\n")
                c = split[1].split("-->")[1]
                t = split[2].split("-->")[1]
                calls.append(float(c))
                times.append(float(t))
            else:
                calls.append(timeout)
                times.append(timeout)

    thread = threading.Thread(target=target)
    thread.start()

    thread.join(timeout)
    if thread.is_alive():
        process.terminate()
        thread.join()

def median(array):
    length = len(array)
    if length % 2 == 1:
        return array[length / 2]
    else:
        return (array[length / 2] + array[length / 2 - 1]) / 2.0

N = [100, 150]
S = ["random", "two", "tiny"]
R = 60

for n in N:
    r = 30
    while r <= 60:
        l = r * n / 10
        r += 2

        sh = "rm tmp.cnf"
        run(sh)
        sh = "./tiny_sat -o tmp.cnf -p " + str(n) + " -c " + str(l) + " -l 3"
        run(sh)
        print sh

        for s in S:
            sh = "./tiny_sat -i tmp.cnf -s " + s
            run(sh)
            print sh

            calls = []
            times = []
            for i in range(10):
                run(sh, calls, times)

            calls.sort()
            times.sort()
            o = "calls: [" + str(calls[0]) + "," + str(median(calls)) + "," + str(calls[-1]) + "]"
            print o
            o = "times: [" + str(times[0]) + "," + str(median(times)) + "," + str(times[-1]) + "]"
            print o
