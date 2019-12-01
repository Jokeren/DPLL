#!/bin/python

import subprocess, threading
import os

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
            command.run(sh, timeout=60.0)
            R += 2
