#!/bin/python

import subprocess, threading
import os
import sys

class Command:
    def __init__(self):
        self._process = None

    def run(self, cmd, calls=None, times=None, timeout=10):
        def target():
            self._process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
            stdout, stderr = self._process.communicate()
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
            self._process.terminate()
            thread.join()

def median(array):
    length = len(array)
    if length % 2 == 1:
        return array[int(length / 2)]
    else:
        return (array[int(length / 2)] + array[int(length / 2) - 1]) / 2.0
    

def report(prefix, calls, times):
    o = prefix + ": calls: [" + str(calls[0]) + "," + str(median(calls)) + "," + str(calls[-1]) + "]"
    os.system("echo " + o)
    o = prefix + ": times: [" + str(times[0]) + "," + str(median(times)) + "," + str(times[-1]) + "]"
    os.system("echo " + o)


N = [150]
S = ["random", "two", "tiny"]
R = 60
cmd = Command()

for n in N:
    r = 30

    while r <= 60:
        l = int(r * n / 10)
        r += 2

        random_calls = []
        random_times = []
        two_calls = []
        two_times = []
        tiny_calls = []
        tiny_times = []

        for i in range(10):
            sh = "rm tmp.cnf"
            os.system(sh)
            sh = "./tiny_sat -o tmp.cnf -p " + str(n) + " -c " + str(l) + " -l 3"
            os.system(sh)

            for s in S:
                sh = "./tiny_sat -i tmp.cnf -s " + s

                if s == "random":
                    calls = random_calls
                    times = random_times
                elif s == "two":
                    calls = two_calls
                    times = two_times
                else:
                    calls = tiny_calls
                    times = tiny_times

                cmd.run(sh, calls, times)

        random_calls.sort()
        random_times.sort()
        two_calls.sort()
        two_times.sort()
        tiny_calls.sort()
        tiny_times.sort()

        sh = "./tiny_sat -p " + str(n) + " -c " + str(l) + " -l 3"
        os.system("echo " + sh)
        report("random", random_calls, random_times)
        report("two", two_calls, two_times)
        report("tiny", tiny_calls, tiny_times)
