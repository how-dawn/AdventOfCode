
from collections import defaultdict
import re
import math


def handleInput(filePath):
    lines = []

    with open(filePath, 'r') as file:
        for line in file:
            line = line.strip()
            line = re.split(r"[:|]", line)
            s1 = line[1].strip().split(" ")
            s2 = line[2].strip().split(" ")
            # idx = line[0].strip().split(" ")[1]
            s1 = [x for x in s1 if x != '']
            s2 = [x for x in s2 if x != '']
            lines.append((s1, s2))
            print((s1, s2))
            print(set(s1), set(s2))
    
    return lines


def solver(lines):
    res = []
    vDict = defaultdict(int)
    for i, line in enumerate(lines):
        vDict[i] += 1
        s1, s2 = line
        c = len(set(s1) & set(s2))

        if c == 0:
            res.append(0)
        elif c > 0:
            res.append(int(2**(c - 1)))


        for j in range(c):
            vDict[(i+1) + j] += vDict[i]
        
        print(vDict)

    return res, vDict
            


def main():
    lines = handleInput("./day4/input.txt")

    res, vDict = solver(lines)
    print(res)
    print(sum(res))

    print(vDict.values())
    print(sum(vDict.values()))

    return


if __name__ == '__main__':
    main()