
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


def search(engine, x, y):
    for direct in directs:
        _x, _y = x + direct[0], y + direct[1]
        if (_x >= 0 and _x < len(engine)) and (_y >= 0 and _y < len(engine[0])-1):
            if not engine[_x][_y].isdigit() and not engine[_x][_y] == '.':
                return True
           
        else:
            continue
            
    return False


def solver(engine: list):
    valid = []
    invalid = []
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

                flag = False
                for k in range(i, j):
                    flag = flag or search(engine, x, k)

                num = int(''.join(map(str, line[i:j])))
                if flag:
                    valid.append(num)
                else:
                    invalid.append(num)

                i = j

    return valid, invalid


if __name__ == '__main__':
    valid, invalid = solver(inputHandler("./day3/input.txt"))
    print(valid)
    print(invalid)
    print("Sum =", sum(valid))
