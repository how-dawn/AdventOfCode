import os

import cv2
import numpy as np
from PIL import Image, ImageDraw, ImageFont
from tqdm import tqdm


def read_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    lines = [line.strip() for line in lines]

    data = np.array([list(s) for s in lines], dtype='U1')
    return data


def get_texts(file_dir):
    texts = [read_file(file_dir + name) for name in
             sorted(os.listdir(file_dir), key=lambda img: int(img.split('_')[-1].split('.')[0]))]
    if not texts:
        print("No images found in the folder!")

    return texts


def text_to_image(text):
    height, width = text.shape

    font_size = 50
    font = ImageFont.load_default(font_size)

    image = Image.new('RGB', (width * font_size, height * font_size), color="black")

    draw = ImageDraw.Draw(image)

    for i in range(height):
        for j in range(width):
            char = text[i, j]
            draw.text((j * font_size, i * font_size), char, font=font, fill="white")
    return image


def write_frame(video_writer, image):
    height, width, _ = np.array(image).shape

    frame = cv2.cvtColor(np.array(image), cv2.COLOR_RGB2BGR)
    video_writer.write(frame)


if __name__ == "__main__":
    base_path = os.path.expanduser('/data/day15/')
    file_dir = base_path + 'output/'

    texts = get_texts(file_dir)
    print('Texts extracted!')

    height, width, _ = np.array(text_to_image(texts[0])).shape

    fps = 10
    output_video_path = base_path + 'day15_visualization_{}.mp4'.format(fps)

    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    video_writer = cv2.VideoWriter(output_video_path, fourcc, fps, (width, height))

    for text in tqdm(texts, desc="Generating video..."):
        image = text_to_image(text)
        write_frame(video_writer, image)

    video_writer.release()
    print(f"Video saved: {output_video_path}")
