#!/bin/python

import subprocess, threading
import os
import sys

class Command:
    def __init__(self):
        self._process = None

    def run(self, cmd, calls=None, times=None, sats=None, timeout=60):
        def target():
            self._process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
            stdout, stderr = self._process.communicate()
            if calls is not None:
                if stdout.find("-->") != -1:
                    split = stdout.split("\n")
                    c = split[1].split("-->")[1]
                    t = split[2].split("-->")[1]
                    if split[0].find("UNSAT") != -1:
                        sats.append("UNSAT")
                    else:
                        sats.append("SAT")
                    calls.append(float(c))
                    times.append(float(t))
                else:
                    sats.append("UNSAT")
                    calls.append(1 << 30)
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
    

def report(prefix, calls, times, sats):
    o = prefix + ": calls: [" + str(calls[0]) + "," + str(median(calls)) + "," + str(calls[-1]) + "]"
    os.system("echo " + o)
    o = prefix + ": times: [" + str(times[0]) + "," + str(median(times)) + "," + str(times[-1]) + "]"
    os.system("echo " + o)
    nsat = 0
    nunsat = 0
    for sat in sats:
        if sat == "SAT":
            nsat = nsat + 1
        else:
            nunsat = nunsat + 1
    o = float(nsat) / float(nsat + nunsat)
    os.system("echo " + str(o))


N = [100, 150]
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
        random_sats = []
        two_calls = []
        two_times = []
        two_sats = []
        tiny_calls = []
        tiny_times = []
        tiny_sats = []

        for i in range(100):
            sh = "rm tmp.cnf"
            os.system(sh)
            sh = "./tiny_sat -o tmp.cnf -p " + str(n) + " -c " + str(l) + " -l 3"
            os.system(sh)

            for s in S:
                sh = "./tiny_sat -i tmp.cnf -s " + s

                if s == "random":
                    calls = random_calls
                    times = random_times
                    sats = random_sats
                elif s == "two":
                    calls = two_calls
                    times = two_times
                    sats = two_sats
                else:
                    calls = tiny_calls
                    times = tiny_times
                    sats = tiny_sats

                cmd.run(sh, calls, times, sats)

        random_calls.sort()
        random_times.sort()
        random_sats.sort()
        two_calls.sort()
        two_times.sort()
        two_sats.sort()
        tiny_calls.sort()
        tiny_times.sort()
        tiny_sats.sort()

        sh = "./tiny_sat -p " + str(n) + " -c " + str(l) + " -l 3"
        os.system("echo " + sh)
        if len(random_calls) > 0:
            report("random", random_calls, random_times, random_sats)
        if len(two_calls) > 0:
            report("two", two_calls, two_times, two_sats)
        if len(tiny_calls) > 0:
            report("tiny", tiny_calls, tiny_times, tiny_sats)
