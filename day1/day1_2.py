import re

def inputHandler(filePath:str):
    ret = []
    with open(filePath, 'r') as file:
        for line in file:
            ret.append(line.strip())
    
    return ret


def extractor(line: str):
    s2d_map = {
        "one": 1,
        "two": 2,
        "three": 3,
        "four": 4,
        "five":5,
        "six":6,
        "seven": 7,
        "eight": 8,
        "nine": 9 
    }

    d2s_map = {1: 'one', 
               2: 'two', 3: 'three', 4: 'four', 5: 'five', 6: 'six', 7: 'seven', 8: 'eight', 9: 'nine'}

    num_rank = {}
    for keyword in s2d_map.keys():
        num_rank[keyword] = [match.start() for match in re.finditer(keyword, line)]

    

    for idx, c in enumerate(line):
        if c.isdigit():
            num_rank[d2s_map[int(c)]].append(idx)

    last_d = ""
    first_d = ""
    last_idx = -1
    first_idx = len(line)

    for k, v in num_rank.items():
        for _v in v:
            if _v > last_idx:
                last_idx = _v
                last_d = k
            if _v < first_idx:
                first_idx = _v
                first_d = k
        
    if first_d == '' or last_d == '':
        print(line)
        return 0
    else:
        return s2d_map[first_d] * 10 + s2d_map[last_d]


def main():
    lines = inputHandler("./day1/input.txt")

    values = [extractor(line) for line in lines]

    test = lines[15]
    print(test, extractor(test))

    print(sum(values))


if __name__ == "__main__":
    main()