import copy
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

def mapGenerator(data):
    maps = []
    seeds = []
    for k, v in data.items():
        if k == "seeds":
           seeds = v[0]
           continue
        maps.append(v)
    return seeds, maps


def seedGenerator(seeds):
    n = len(seeds)
    pairs = list(zip(seeds[::2], seeds[1::2]))

    res = []
    for p in pairs:
        s, c = p
        _res = [s + i for i in range(c)]
        res += _res
    return res


def solver(seeds, maps):
    res = copy.deepcopy(seeds)

    for m in maps:
        _reslist = []
        for _m  in m:
            dst, src, c = _m
            _res = copy.deepcopy(res)
            for i, s in enumerate(_res):  
                if s >= src and s <= (src + c) -1:
                    _res[i] = dst + (s - src)
                else:
                    _res[i] = s
            _reslist.append(_res)

        tres = copy.deepcopy(res)

        for lst in _reslist:
            for i, e in enumerate(lst):
                if e != res[i]:
                    if tres[i] == res[i]:
                        tres[i] = e
    
        res  = tres

    return res
             
def main():
    data = inputHandler(filePath="./day5/input.txt")
    seeds, maps = mapGenerator(data)
    seeds = seedGenerator(seeds)
    print("seeds:", seeds)
    res = solver(seeds, maps)
    print("ans:", min(res))


if __name__ == '__main__':
    main()
