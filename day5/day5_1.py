import re


def inputHandler(filePath):
    data = {}
    with open(filePath, 'r') as file:
        file = file.read().strip()
        file = file.split("\n\n")

        for section in file:
            section = [s.strip() for s in section.split(":")]
            k, t = section
            t = t.split("\n")
            v = [_t.split(" ") for _t in t]
            v = [[int(_x) for _x in _v] for _v in v]
            data[k] = v

    return data


def _mapGenerator(k, v):
    rMap = {}
    for _v in v:
        d, s, c = _v
        for i in range(c):
            rMap[s + i] = d + i
    
    return rMap

def mapGenerator(data):
    maps = []
    seeds = []
    for k, v in data.items():
        if k == "seeds":
           seeds = v[0]
           continue
        _map = _mapGenerator(k, v)
        maps.append(_map)
    return seeds, maps

def solver(seeds, maps):
    res = seeds
    for m in maps:
        for i, s in enumerate(res):
            if s in m.keys():
                res[i] = m[s]
            else:
                res[i] = s
        print(res)
    return res
                
def main():
    data = inputHandler(filePath="./day5/test.txt")
    # print(data)
    seeds, maps = mapGenerator(data)
    print("seeds:", seeds)
    res = solver(seeds, maps)
    # print("res: ", res)
    print("ans:", min(res))


if __name__ == '__main__':
    main()
