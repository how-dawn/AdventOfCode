import copy
from multiprocessing import Pool


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


def dataProcessor(data):
    maps = []
    seeds = []
    for k, v in data.items():
        if k == "seeds":
            seeds = v[0]
            continue
        maps.append(v)
    pairs = list(zip(seeds[::2], seeds[1::2]))
    pairs = [[s, s + e] for (s, e) in pairs] 
    return pairs, maps


def solver(pairs, maps):
    ret = []
    for p in pairs:
        ranges = [p]
        res = []
        for _m in maps:
            while ranges:
                l, u  = ranges.pop()
                flag = False
                for (d, s, c) in _m:
                    s_l, s_u = s, s + c
                    offset = d - s
                    
                    if s_u <= l or s_l >=u:
                        continue

                    if l < s_l:
                        ranges.append([l, s_l])
                        l = s_l
                    
                    if u > s_u:
                        ranges.append([s_u, u]) 
                        u = s_u
                    
                    res.append([l + offset, u + offset])
                    flag = True
                    break
                
                if not flag:
                    res.append([l, u])
            
            ranges = res
            res = []
        ret += ranges

    return ret                


def main():
    data = inputHandler(filePath="./day5/input.txt")
    seeds, maps = dataProcessor(data)
    res = solver(seeds, maps)
    print("ans:", min(r[0] for r in res))


if __name__ == '__main__':
    main()
