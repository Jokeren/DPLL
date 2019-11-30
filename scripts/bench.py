#!/bin/python

import subprocess, threading

class Command(object):
    def __init__(self):
        self.process = None

    def run(self, cmd, timeout):
        def target():
            self.process = subprocess.Popen(cmd, shell=True)
            self.process.communicate()

        thread = threading.Thread(target=target)
        thread.start()

        thread.join(timeout)
        if thread.is_alive():
            self.process.terminate()
            thread.join()

N = [100, 150]
S = ["random", "two", "tiny"]
command = Command()

for n in N:
    for s in S:
        R = 3.0
        while R <= 6.0:
            L = int(R * n)
            P = "p" + str(n) + "c" + str(L) + "l3.cnf"
            sh = "time ./tiny_sat -i extern/data/" + "p" + str(n) + "/" + P + " -s " + s
            print sh
            command.run(sh, timeout=10.0);
            R += 0.2
