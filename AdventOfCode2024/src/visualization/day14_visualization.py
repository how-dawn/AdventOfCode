import os

import cv2
import numpy as np
from PIL import Image


def read_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
    width = len(lines[0].strip())

    binary_data = ''.join(line.strip() for line in lines)
    data = np.array([int(b) for b in binary_data], dtype=np.uint8).reshape(-1, width)
    return data.T * 255


def get_images(image_dir):
    images = [read_file(image_dir + img) for img in
              sorted(os.listdir(image_dir), key=lambda img: int(img.split('_')[-1].split('.')[0]))]
    if not images:
        print("No images found in the folder!")

    return images


def images_to_video(images, output_video_path, frame_rate=30):
    height, width = images[0].shape

    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    video_writer = cv2.VideoWriter(output_video_path, fourcc, frame_rate, (width, height), False)

    for image in images:
        video_writer.write(image)

    video_writer.release()
    print(f"Video saved: {output_video_path}")


def save_image(image, image_output_path):
    image = Image.fromarray(image)
    image.save(image_output_path)
    print(f"Image saved: {image_output_path}")


if __name__ == "__main__":
    base_path = os.path.expanduser('/data/day14/')
    image_dir = base_path + 'output/'

    images = get_images(image_dir)

    output_image_path = base_path + 'day14_easter_egg.png'
    save_image(images[-1], output_image_path)

    fps = 5
    output_video_path = base_path + 'day14_easter_egg_{}.mp4'.format(fps)
    images_to_video(images, output_video_path, fps)

    fps = 15
    output_video_path = base_path + 'day14_easter_egg_{}.mp4'.format(fps)
    images_to_video(images, output_video_path, fps)

    fps = 30
    output_video_path = base_path + 'day14_easter_egg_{}.mp4'.format(fps)
    images_to_video(images, output_video_path, fps)

    fps = 60
    output_video_path = base_path + 'day14_easter_egg_{}.mp4'.format(fps)
    images_to_video(images, output_video_path, fps)
