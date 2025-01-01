import sys
import re


def read_input(file_path: str):
    with open(file_path, 'r') as file:
        data = [line.strip() for line in file]

    return data


def part1(data: list):
    res = []
    for line in data:
        nums = re.findall(r'\d', line)
        if len(nums) == 0:
            res.append(0)
            continue
        res.append(int(nums[0] + nums[-1]))

    return sum(res)


def part2(data: list):
    str_to_int = {
        "one": 1,
        "two": 2,
        "three": 3,
        "four": 4,
        "five": 5,
        "six": 6,
        "seven": 7,
        "eight": 8,
        "nine": 9
    }

    keys = str_to_int.keys()
    pattern = r"(?=({}|\d))".format('|'.join(keys))
    print(pattern)

    res = []
    for line in data:
        match = re.findall(pattern, line)
        if len(match) == 0:
            res.append(0)
            continue

        match_first, match_last = match[0], match[-1]
        num_first = int(match_first) if match_first.isdigit() else str_to_int[match_first]
        num_last = int(match_last) if match_last.isdigit() else str_to_int[match_last]

        num = num_first * 10 + num_last

        print(line, match, num)
        res.append(num)

    return sum(res)


if __name__ == "__main__":
    input_data = read_input(sys.argv[1])
    print("Part1: ", part1(input_data))
    print("Part2: ", part2(input_data))
