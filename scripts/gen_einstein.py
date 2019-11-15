#!/bin/python

# check 14, 39, 64, 89[Y], 114 for fish
# check 1-5, 26-30, 51-55, 76-80[Y], 101-105 for house
# check 76-100 for other properties
# !first house
# 4 6 12 16 24
# norwegian yellow cat water dunhill
# !second house
# 26 35 36 42 50 
# dane blue horse tea blends
# !third house
# 52 57 63 68 71 
# brit red brid milk pallmall
# !fourth house
# 80 84 89 94 97
# green fish coffee prince fifth house
# !fifth house
# 103 108 115 120 123
# swede white dog rootbeer bluemaster

entities = dict({
    "dane" : 0, "brit" : 1, "swede" : 2, "norwegian" : 3, "german" : 4,
    "yellow" : 5, "red" : 6, "white" : 7, "green" : 8, "blue" : 9,
    "horse" : 10, "cat" : 11, "bird" : 12, "fish" : 13, "dog" : 14,
    "water" : 15, "tea" : 16, "milk" : 17, "coffee" : 18, "root beer" : 19,
    "pall mall" : 20, "prince" : 21, "blue master" : 22, "dunhill" : 23, "blends" : 24
});

def p(house, entity):
    if type(entity) is str:
        return house * 25 + entities[entity] + 1
    elif type(entity) is int:
        return house * 25 + entity + 1
    else:
        return None

def forall(e1, e2):
    ret = []
    for i in range(5):
        ret.append([p(i, e1), p(i, e2)])
    return ret

def insequence(e1, e2):
    ret = []
    for i in range(4):
        ret.append([p(i, e1), p(i+1, e2)])
    return ret

def nextto(e1, e2):
    ret = []
    for i in range(4):
        ret.append([p(i, e1), p(i+1, e2)])
        ret.append([p(i+1, e1), p(i, e2)])
    return ret

def dnf2cnf(dnf):
    if len(dnf) < 2:
        return None
    cnf = []
    for i in range(len(dnf)):
        if i == 0:
            for d in dnf[0]:
                cnf += [[d]]
        else:
            tmp = []
            for c in cnf:
                for d in dnf[i]:
                    tmp.append(c + [d])
            cnf = tmp
    return cnf

def f_unique_entity(e):
    return [[p(0, e), -p(1, e), -p(2, e), -p(3, e), -p(4, e)], 
            [-p(0, e), p(1, e), -p(2, e), -p(3, e), -p(4, e)],
            [-p(0, e), -p(1, e), p(2, e), -p(3, e), -p(4, e)],
            [-p(0, e), -p(1, e), -p(2, e), p(3, e), -p(4, e)],
            [-p(0, e), -p(1, e), -p(2, e), -p(3, e), p(4, e)]]

def f_unique_house(i, g):
    return [[p(i, g*5), -p(i, g*5+1), -p(i, g*5+2), -p(i, g*5+3), -p(i, g*5+4)], 
            [-p(i, g*5), p(i, g*5+1), -p(i, g*5+2), -p(i, g*5+3), -p(i, g*5+4)], 
            [-p(i, g*5), -p(i, g*5+1), p(i, g*5+2), -p(i, g*5+3), -p(i, g*5+4)], 
            [-p(i, g*5), -p(i, g*5+1), -p(i, g*5+2), p(i, g*5+3), -p(i, g*5+4)], 
            [-p(i, g*5), -p(i, g*5+1), -p(i, g*5+2), -p(i, g*5+3), p(i, g*5+4)]]

cnf = []
cnf += dnf2cnf(forall("brit", "red"))
cnf += dnf2cnf(forall("swede", "dog"))
cnf += dnf2cnf(forall("dane", "tea"))
cnf += dnf2cnf(insequence("green", "white"))
cnf += dnf2cnf(forall("green", "coffee"))
cnf += dnf2cnf(forall("pall mall", "bird"))
cnf += dnf2cnf(forall("yellow", "dunhill"))
cnf += [[p(2, "milk")]]
cnf += [[p(0, "norwegian")]]
cnf += dnf2cnf(nextto("blends", "cat"))
cnf += dnf2cnf(nextto("horse", "dunhill"))
cnf += dnf2cnf(forall("blue master", "root beer"))
cnf += dnf2cnf(forall("german", "prince"))
cnf += dnf2cnf(nextto("norwegian", "blue"))
cnf += dnf2cnf(nextto("blends", "water"))

for i in range(25):
    cnf += dnf2cnf(f_unique_entity(i))

for i in range(5):
    for j in range(5):
        cnf += dnf2cnf(f_unique_house(i, j))

# to dimacs
propositions = 125
clauses = len(cnf)

f = open("extern/data/einstein.cnf", "w")
f.write("c einstein\n")
f.write("p cnf " + str(propositions) + " " + str(clauses) + "\n")
buf = ""
for c in cnf:
    for l in c:
        buf += str(l) + " "
    buf += "0\n"
f.write(buf)
f.close()
