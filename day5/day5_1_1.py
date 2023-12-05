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

        print("res:", res)
        for _ in _reslist:
            print(_)


        tres = copy.deepcopy(res)

        for lst in _reslist:
            for i, e in enumerate(lst):
                if e != res[i]:
                    if tres[i] == res[i]:
                        tres[i] = e
        
        print("--", tres)
        res  = tres
        
       
        # tres = [_ for _ in res]
        # for __res in _reslist:
        #     for i, e in enumerate(__res):
        #         if e != tres[i]:
        #             if tres[i] == res[i]:
        #                 tres[i] = e
                
                    
        # print("-", tres)
        print("=============")
        # res = tres

        # for i, e in tres:
        #     if e != seeds[i]:
        #         if 
        
        # print("===", res)


    return res
                
def main():
    data = inputHandler(filePath="./day5/test.txt")
    # print(data)
    seeds, maps = mapGenerator(data)
    print("seeds:", seeds)
    print("maps:\n", maps)
    print("\n")
    res = solver(seeds, maps)
    print("res: ", res)
    print("ans:", min(res))


if __name__ == '__main__':
    main()
