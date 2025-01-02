import re
import sys


def read_input(file_path: str):
    with open(file_path, 'r') as file:
        data = [line.strip().split(':')[-1].strip() for line in file]
        data = [line.split(';') for line in data]

    pattern = r'(\d+ red|\d+ green|\d+ blue)'

    def parse_rgb_set(rgb_set):
        red, green, blue = 0, 0, 0
        for rgb in re.findall(pattern, rgb_set):
            value, color = rgb.split()
            if color == "red":
                red = int(value)
            elif color == "green":
                green = int(value)
            elif color == "blue":
                blue = int(value)
        return [red, green, blue]

    data = [[parse_rgb_set(rgb_set) for rgb_set in game] for game in data]

    return data


def part1(data: list):
    valid = (12, 13, 14)

    def rgb_check(rgb_tuple: tuple):
        return all(rgb_tuple[i] <= v for i, v in enumerate(valid))

    check_res = filter(lambda i_res: all(i_res[-1]),
                       [(i, [rgb_check(rgb_tuple) for rgb_tuple in game]) for i, game in enumerate(data)])

    return sum([i + 1 for i, _ in check_res])


def part2(data: list):
    def get_maximum(game):
        rgb_max = [0, 0, 0]
        for r, g, b in game:
            rgb_max[0] = max(rgb_max[0], r)
            rgb_max[1] = max(rgb_max[1], g)
            rgb_max[2] = max(rgb_max[2], b)
        return rgb_max

    max_game = [get_maximum(game) for game in data]
    max_game_power = [r * g * b for r, g, b in max_game]

    return sum(max_game_power)


if __name__ == "__main__":
    input_data = read_input(sys.argv[1])
    print("Part1: ", part1(input_data))
    print("Part2: ", part2(input_data))
