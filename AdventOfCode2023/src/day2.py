import re
import sys


def read_input(file_path: str):
    pattern = r'(\d+) (red|green|blue)'

    def parse_rgb_set(rgb_set):
        counts = {color: int(value) for value, color in re.findall(pattern, rgb_set)}
        return [int(counts.get(color, 0)) for color in ("red", "green", "blue")]

    with open(file_path, 'r') as file:
        return [[parse_rgb_set(rgb_set) for rgb_set in line.split(':')[-1].strip().split(';')]
                for line in file]


def part1(data: list):
    return sum(i + 1 for i, game in enumerate(data) if
               all(all(rgb <= v for rgb, v in zip(rgb_tuple, (12, 13, 14))) for rgb_tuple in game))


def part2(data: list):
    return sum(r * g * b for game in data for r, g, b in [map(max, zip(*game))])


if __name__ == "__main__":
    input_data = read_input(sys.argv[1])
    print("Part1: ", part1(input_data))
    print("Part2: ", part2(input_data))
