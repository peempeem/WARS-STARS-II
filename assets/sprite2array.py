import os
import shutil
from PIL import Image
import numpy as np

input_folder = "./sprites"
output_folder = "../src/software/ws2/sprites"


def write_header(name, width, height):
    with open(f"{output_folder}/{name}.h", 'w') as f:
        f.write(
f"""// THIS IS AN AUTOGENERATED FILE

#ifndef {name.upper()}_H
#define {name.upper()}_H

#include <stdint.h>

const static int {name}_width   = {width};
const static int {name}_height  = {height};

const static uint16_t {name}_bitmap[] = {{ """)
        count = 0
        for i in range(len(data)):
            if count % width == 0:
                f.write('\n\t')
            f.write("0x%0.4x, " % data[i])
            count += 1
        f.write(
f"""
}};

#endif
""")


if __name__ == '__main__':
    # check if input folder exists
    if not os.path.isdir(input_folder):
        print("Input folder is empty.")
        quit()

    # check if input folder contains any images
    files = os.listdir(input_folder)
    print(files)
    for file in files:
        if not ".png" in file:
            files.remove(file)
    print(f"Images found: {len(files)}")
    if len(files) == 0:
        quit()

    # clear output folder
    if os.path.isdir(output_folder):
        shutil.rmtree(output_folder)
    os.mkdir(output_folder)

    for file in files:
        image = Image.open(f"{input_folder}/{file}", 'r')

        if image.mode != "RGBA":
            print(f"Warning {file} is not RGBA")
            image = image.convert("RGBA")

        width, height = image.size
        data = []
        for y in range(height):
            for x in range(width):
                r, g, b, a = image.getpixel((x, y))
                pxl = int(a == 255) << 4
                pxl = pxl + int(16 * r / 255) << 4
                pxl = (pxl + int(16 * g / 255)) << 4
                pxl += int(16 * b / 255)
                data.append(pxl)
        name = file.split('.')[0]
        write_header(name, width, height)

        print(f"Converted {file}")