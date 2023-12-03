import re

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
        flag = True
        for elem in line:
            if elem[0] <= standard[0] and elem[1] <= standard[1] and elem[2] <= standard[2]:
                continue
            else:
                flag = False
                break
        if flag: valid.append(id)
    return valid



def main():
    lines = inputHandler(filePath="day2/input.txt")
    valid = check(lines)
    print(valid)
    print(sum(valid))

if __name__ == '__main__':
    main()