
from functools import reduce


directs = [
    [1, 0], [-1, 0], [0, -1], [0, 1],
    [-1, -1], [1, -1], [-1, 1], [1, 1]
]


def inputHandler(filePath: str):
    engine = []
    with open(filePath, 'r') as file:
        _engine = []
        for line in file:
            _engine = [c for c in line.strip()]
            engine.append(_engine)
    return engine


def search(engine: list, x: int, y: int):
    flag = False
    gears = []
    for direct in directs:
        _x, _y = x + direct[0], y + direct[1]
        if (_x >= 0 and _x < len(engine)) and (_y >= 0 and _y < len(engine[0])-1):
            if not engine[_x][_y].isdigit() and not engine[_x][_y] == '.':
                gears.append((_x, _y))
                flag = True
           
        else:
            continue
            
    return flag, gears


def solver(engine: list):
    gearMap = {}

    for x, line in enumerate(engine):
        lLen = len(line)
        i = 0
        while (i < lLen):
            
            c = line[i]
            if not c.isdigit():
                i += 1
            else:
                j = i + 1
                while (j < lLen):
                    if line[j].isdigit():
                        j += 1
                    else:
                        break

                for k in range(i, j):
                    _flag, _gear = search(engine, x, k)
                    
                    for g in _gear:
                        if g in gearMap.keys():
                            gearMap[g].add((x, i, j))
                        else:
                            gearMap[g] = {(x, i, j)}


                i = j
    
    res = {}
    for k, v in gearMap.items():
        res[k] = []
        for t in v:
            x, i, j = t
            print(i, j)
            num = int(''.join(map(str, engine[x][i:j])))
            res[k].append(num)

    return res


if __name__ == '__main__':
    gearMap = solver(inputHandler("./day3/input.txt"))

    res = []
    for k, v in gearMap.items():
        v = list(v)
        if len(v) == 2:
            res.append(v[1] * v[0])

    print(res)
    print("sum = {}".format(sum(res)))