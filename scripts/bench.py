#!/bin/python

import subprocess, threading
import os
import sys

class Command(object):
    def __init__(self):
        self.process = None

    def run(self, cmd, calls, times, timeout):
        def target():
            self.process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
            stdout, stderr = self.process.communicate()
            if stdout.find("-->") != -1:
                split = stdout.split("\n")
                c = split[1].split("-->")[1]
                t = split[2].split("-->")[1]
                calls.append(float(c))
                times.append(float(t))
            else:
                calls.append(1 << 30)
                calls.append(1 << 30)

        thread = threading.Thread(target=target)
        thread.start()

        thread.join(timeout)
        if thread.is_alive():
            self.process.terminate()
            thread.join()

def median(array):
    length = len(array)
    if length % 2 == 1:
        return array[length / 2]
    else:
        return (array[length / 2] + array[length / 2 - 1]) / 2.0

N = [125, 150]
S = ["random", "two", "tiny"]
command = Command()

for n in N:
    for s in S:
        R = 30
        while R <= 60:
            L = R * n / 10
            P = "p" + str(n) + "c" + str(L) + "l3.cnf"
            sh = "./tiny_sat -i extern/data/" + "p" + str(n) + "/" + P + " -s " + s
            os.system("echo " + sh)

            calls = []
            times = []
            if s == "random":
                for i in range(10):
                    command.run(sh, calls, times, timeout=60.0)
            else:
                for i in range(100):
                    command.run(sh, calls, times, timeout=60.0)

            calls.sort()
            times.sort()
            o = "calls: [" + str(calls[0]) + "," + str(median(calls)) + "," + str(calls[-1]) + "]"
            os.system("echo " + o)
            o = "times: [" + str(times[0]) + "," + str(median(times)) + "," + str(times[-1]) + "]"
            os.system("echo " + o)
                
            R += 2
