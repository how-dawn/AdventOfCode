from functools import reduce
import re
import sys

INT_MAX = sys.maxsize
INT_MIN = -sys.maxsize - 1


c2n = {
    'red': 0,
    'green': 1,
    'blue': 2
}

n2c = {v: k for k, v in c2n.items()}

def inputHandler(filePath: str):
    lines = {}
    with open(filePath, 'r') as file:
        for line in file:
            line = re.split(r"[:;]", line)
            id = int(line[0].split(" ")[1])
            groups = []
            for group in line[1:]:
                res = [0, 0, 0]
                elements = [e.strip() for e in group.split(",")]
                for unit in elements:
                    v, k = unit.split(" ")
                    v = int(v)
                    res[c2n[k]] += v
                groups.append(res)
            lines[id] = groups 
    return lines

def check(lines, standard=[12, 13, 14]):
    valid = []
    for id, line in lines.items():
        bounds = [INT_MIN for _ in range(3)]
        for elem in line:
            bounds = [
                max(bounds[0], elem[0]),
                max(bounds[1], elem[1]),
                max(bounds[2], elem[2]),
            ]
        valid.append(bounds)
    return valid



def main():
    lines = inputHandler(filePath="day2/input.txt")
    valid = check(lines)
    ret = 0
    for nums in valid:
        result = reduce(lambda x, y: x * y, nums)
        ret += result
    print(ret)

if __name__ == '__main__':
    main()