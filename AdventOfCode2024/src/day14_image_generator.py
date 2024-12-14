import os

import numpy as np
from PIL import Image


def read_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
    width = len(lines[0].strip())

    binary_data = ''.join(line.strip() for line in lines)
    data = np.array([int(b) for b in binary_data], dtype=np.uint8).reshape(-1, width)

    return data.T


def create_image(data):
    return Image.fromarray(data * 255)


if __name__ == "__main__":
    base_path = os.path.expanduser('~/Workspace/Dev/AdventOfCode/AdventOfCode2024/data/day14/')
    file_path = base_path + 'day14_easter_egg.output'
    output_path = base_path + 'output_image.png'

    data = read_file(file_path)

    image = create_image(data)
    image.save(output_path)
    print("Image saved: ", output_path)
