
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
    for line in lines:
        s1, s2 = line
        _c = len(set(s1) & set(s2))

        if _c == 0:
            res.append(0)
        elif _c > 0:
            res.append(int(2**(_c - 1)))
    return res



def main():
    lines = handleInput("./day4/input.txt")

    res = solver(lines)
    print(sum(res))

    return


if __name__ == '__main__':
    main()